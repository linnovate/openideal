<?php

namespace Drupal\openideal_faq\Plugin\paragraphs\Behavior;

use Drupal\Core\Entity\Display\EntityViewDisplayInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\paragraphs\Entity\ParagraphsType;
use Drupal\paragraphs\ParagraphInterface;
use Drupal\paragraphs\ParagraphsBehaviorBase;

/**
 * Provides a paragraphs behaviours for FAQ field view format.
 *
 * @ParagraphsBehavior(
 *   id = "faq_view_behaviour",
 *   label = @Translation("FAQ's view format"),
 *   description = @Translation("Choose the view format you need: accordion or list of anchors"),
 *   weight = 1
 * )
 */
class FAQViewFormatBehaviour extends ParagraphsBehaviorBase {

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return [
      'default_view' => 'faqfield_accordion',
      'formats' => [
        'faqfield_accordion' => 'Accordion',
        'faqfield_anchor_list' => 'Anchor list',
      ],
    ];
  }

  /**
   * {@inheritDoc}
   */
  public static function isApplicable(ParagraphsType $paragraphs_type) {
    return $paragraphs_type->id() == 'faq';
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form['view_format'] = [
      '#type' => 'select',
      '#title' => $this->t('Choose view format'),
      '#default_value' => $this->configuration['default_view'],
      '#options' => $this->configuration['formats'],
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    $this->configuration['default_view'] = $form_state->getValue('view_format');
  }

  /**
   * {@inheritdoc}
   */
  public function buildBehaviorForm(ParagraphInterface $paragraph, array &$form, FormStateInterface $form_state) {
    $form['view_format'] = [
      '#type' => 'select',
      '#options' => $this->configuration['formats'],
      '#default_value' => $paragraph->getBehaviorSetting($this->getPluginId(), 'view_format', $this->configuration['default_view']),
      '#description' => $this->t('View format for FAQ.'),
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function view(array &$build, Paragraph $paragraphs_entity, EntityViewDisplayInterface $display, $view_mode) {
    // If FAQ formatter has been changed then apply new formatter to it.
    if (($default_view = $paragraphs_entity->getBehaviorSetting($this->getPluginId(), 'view_format', $this->configuration['default_view']))
      && $default_view != $display->getComponent('field_faq_items')['type']) {
      $display->setComponent('field_faq_items', ['type' => $default_view]);
      $display->save();
      // Replace old renderable array with new one.
      $build = $display->build($paragraphs_entity);
    }
  }

}
