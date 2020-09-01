<?php

namespace Drupal\openideal_footer\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class OpenidealFooterConfigForm.
 */
class OpenidealFooterConfigForm extends ConfigFormBase {

  /**
   * Default social links.
   *
   * Todo: Ask for twitter link.
   */
  const TWITTER = '';
  const GITHUB = 'https://github.com/istolar/openideal_distribution';

  /**
   * The openideal official site.
   */
  const OPENIDEAL_OFFICIAL_SITE = 'https://www.openidealapp.com/';

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'openideal_footer.openideal_footer_links_config',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'openideal_footer_links_config_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('openideal_footer.openideal_footer_links_config');

    $form['twitter'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Twitter'),
      '#description' => $this->t('Openideal twitter official link'),
      '#default_value' => $config->get('twitter') ?? self::TWITTER,
    ];

    $form['github'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Github'),
      '#description' => $this->t('Openideal github repository official link'),
      '#default_value' => $config->get('github') ?? self::GITHUB,
    ];

    $form['openideal_official_site'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Openideal official site'),
      '#default_value' => $config->get('openideal_official_site') ?? self::OPENIDEAL_OFFICIAL_SITE,
    ];

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    parent::submitForm($form, $form_state);

    $this->config('openideal_footer.openideal_footer_links_config')
      ->set('twitter', $form_state->getValue('twitter'))
      ->set('github', $form_state->getValue('github'))
      ->set('openideal_official_site', $form_state->getValue('openideal_official_site'))
      ->save();
  }

}
