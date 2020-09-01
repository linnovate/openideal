<?php

namespace Drupal\openideal_idea\Plugin\Validation\Constraint;

use Drupal\content_moderation\Plugin\Validation\Constraint\ModerationStateConstraintValidator;
use Drupal\Core\Validation\Plugin\Validation\Constraint\NotNullConstraint;
use Symfony\Component\Validator\Constraint;

/**
 * Checks if a moderation state transition is valid.
 *
 * Override default moderation state validator, to implemented additional
 * logic if the previous state equal new state then do not restrict access.
 */
class OpenidealModerationStateConstraintValidator extends ModerationStateConstraintValidator {

  /**
   * {@inheritdoc}
   */
  public function validate($value, Constraint $constraint) {
    /** @var \Drupal\Core\Entity\ContentEntityInterface $entity */
    $entity = $value->getEntity();

    // Ignore entities that are not subject to moderation anyway.
    if (!$this->moderationInformation->isModeratedEntity($entity)) {
      return;
    }

    // If the entity is moderated and the item list is empty, ensure users see
    // the same required message as typical NotNull constraints.
    if ($value->isEmpty()) {
      $this->context->addViolation((new NotNullConstraint())->message);
      return;
    }

    $workflow = $this->moderationInformation->getWorkflowForEntity($entity);

    if (!$workflow->getTypePlugin()->hasState($entity->moderation_state->value)) {
      // If the state we are transitioning to doesn't exist, we can't validate
      // the transitions for this entity further.
      $this->context->addViolation($constraint->invalidStateMessage, [
        '%state' => $entity->moderation_state->value,
        '%workflow' => $workflow->label(),
      ]);
      return;
    }

    $new_state = $workflow->getTypePlugin()->getState($entity->moderation_state->value);
    $original_state = $this->moderationInformation->getOriginalState($entity);

    // If previous state is the same as new state nothing to do here.
    if ($original_state->id() == $new_state->id()) {
      return;
    }
    // If a new state is being set and there is an existing state, validate
    // there is a valid transition between them.
    elseif (!$original_state->canTransitionTo($new_state->id())) {
      $this->context->addViolation($constraint->message, [
        '%from' => $original_state->label(),
        '%to' => $new_state->label(),
      ]);
    }
    else {
      // If we're sure the transition exists, make sure the user has permission
      // to use it.
      if (!$this->stateTransitionValidation->isTransitionValid($workflow, $original_state, $new_state, $this->currentUser, $entity)) {
        $this->context->addViolation($constraint->invalidTransitionAccess, [
          '%original_state' => $original_state->label(),
          '%new_state' => $new_state->label(),
        ]);
      }
    }
  }

}
