<?php

namespace Drupal\openideal_user\Plugin\Condition;

use Drupal\Core\Condition\ConditionPluginBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Routing\CurrentRouteMatch;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'User Page' condition.
 *
 * @Condition(
 *   id = "openideal_user_page",
 *   label = @Translation("User Page"),
 *   context_definitions = {
 *     "user" = @ContextDefinition("entity:user", label = @Translation("User"))
 *   }
 * )
 */
class UserPageCoreCondition extends ConditionPluginBase implements ContainerFactoryPluginInterface {

  /**
   * The CurrentRouteMatch service.
   *
   * @var \Drupal\Core\Routing\CurrentRouteMatch
   */
  protected $currentRouteMatch;

  /**
   * Constructs a Route condition plugin.
   *
   * @param \Drupal\Core\Routing\CurrentRouteMatch $current_route_match
   *   The CurrentRouteMatch service.
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param array $plugin_definition
   *   The plugin implementation definition.
   */
  public function __construct(CurrentRouteMatch $current_route_match, array $configuration, $plugin_id, array $plugin_definition) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->currentRouteMatch = $current_route_match;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $container->get('current_route_match'),
      $configuration,
      $plugin_id,
      $plugin_definition
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form['is_profile_page'] = [
      '#type' => 'checkbox',
      '#title' => $this->t("Checks if it's a user page"),
      '#default_value' => $this->configuration['is_profile_page'],
    ];
    return parent::buildConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return [
      'is_profile_page' => FALSE,
    ] + parent::defaultConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    $this->configuration['is_profile_page'] = (bool) $form_state->getValue('is_profile_page');
    parent::submitConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function summary() {
    if ($this->configuration['negate']) {
      return $this->t("The page is the user's page");
    }
    else {
      return $this->t("The page is not the user's page");
    }
  }

  /**
   * {@inheritdoc}
   */
  public function evaluate() {
    if ((empty($this->configuration['is_profile_page']) && !$this->isNegated())
      || $this->currentRouteMatch->getRouteName() != 'entity.user.canonical'
    ) {
      return TRUE;
    }
    $user = $this->getContextValue('user');

    $route_user = $this->currentRouteMatch->getParameter('user');
    return $user->id() == $route_user->id();
  }

  /**
   * {@inheritdoc}
   */
  public function getCacheContexts() {
    // Optimize cache context, if a user cache context is provided, only use
    // user.is_user_profile_page,
    // since that's the only part this condition cares about.
    $contexts = [];
    foreach (parent::getCacheContexts() as $context) {
      $contexts[] = $context == 'user' ? 'user.is_user_profile_page' : $context;
    }
    return $contexts;
  }

}
