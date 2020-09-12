<?php

namespace Drupal\openideal_statistics\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class OpenidealStatisticsDateSelectForm.
 */
class OpenidealStatisticsDateSelectForm extends FormBase {

  /**
   * The query params to indicate the date filters.
   */
  const FIXED_RANGE = 'range';
  const DATE_TYPE = 'date_type';
  const DATE_TYPE_CUSTOM = 'custom';
  const DATE_TYPE_FIXED = 'fixed';
  const FROM = 'from';
  const TO = 'to';

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'openideal_statistics_date_select_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $query = $this->getRequest()->query;
    $options = [];
    for ($month = 1; $month <= 12; $month++) {
      $options[$month] = $this->formatPlural($month, '1 month', '@count months');
    }

    $form['filters_wrapper'] = [
      '#type' => 'container',
      '#attributes' => ['class' => ['reports-filters']],
    ];
    $form['filters_wrapper'][self::DATE_TYPE] = [
      '#title' => $this->t('Date type'),
      '#title_display' => 'invisible',
      '#type' => 'radios',
      '#attributes' => ['class' => ['reports-filters--date-type']],
      '#options' => [
        'fixed' => $this->t('Fixed filter'),
        'custom' => $this->t('Range filter'),
      ],
      '#default_value' => $query->get(self::DATE_TYPE) ?? NULL,
      '#required' => TRUE,
    ];
    $form['filters_wrapper']['filter_container'] = [
      '#type' => 'container',
      '#attributes' => ['class' => ['reports-filters--filters']],
      'fixed_dates' => [
        '#type' => 'container',
        '#attributes' => ['class' => ['reports-filters--filters__custom_dates']],
      ],
    ];

    $form['filters_wrapper']['filter_container']['fixed_dates'][self::FIXED_RANGE] = [
      '#type' => 'select',
      '#prefix' => '<div class="reports-filters--filters__fixed_dates--label">' . $this->t('From') . '</div>',
      '#options' => $options,
      '#default_value' => $query->get(self::FIXED_RANGE) ?? NULL,
    ];

    $form['filters_wrapper']['filter_container']['custom_dates'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['reports-filters--filters__custom_dates']],
    ];
    $form['filters_wrapper']['filter_container']['custom_dates'][self::FROM] = [
      '#type' => 'date',
      '#prefix' => '<div class="reports-filters--filters__custom_dates--label">' . $this->t('From') . '</div>',
      '#default_value' => $query->get(self::FROM) ?? NULL,
    ];
    $form['filters_wrapper']['filter_container']['custom_dates'][self::TO] = [
      '#type' => 'date',
      '#prefix' => '<div class="reports-filters--filters__custom_dates--label to">' . $this->t('To') . '</div>',
      '#default_value' => $query->get(self::TO) ?? NULL,
    ];

    $form['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Update'),
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->getValues();
    if ($values[self::DATE_TYPE] == self::DATE_TYPE_FIXED && !$values[self::FIXED_RANGE]) {
      $form_state->setErrorByName(self::FIXED_RANGE, $this->t('An option should be selected'));
    }
    elseif ($values[self::DATE_TYPE] == self::DATE_TYPE_CUSTOM && (!$values[self::FROM] && !$values[self::TO])) {
      $form_state->setErrorByName(self::FROM, $this->t('You must specify both dates'));
      $form_state->setErrorByName(self::TO, $this->t('You must specify both dates'));
    }
    elseif ($values[self::DATE_TYPE] == self::DATE_TYPE_CUSTOM && !$values[self::FROM]) {
      $form_state->setErrorByName(self::FROM, $this->t('Field cannot be empty'));
    }
    elseif ($values[self::DATE_TYPE] == self::DATE_TYPE_CUSTOM && !$values[self::TO]) {
      $form_state->setErrorByName(self::TO, $this->t('Field cannot be empty'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->getValues();
    $query = $this->getRequest()->query;
    // Clear query.
    $query->replace([]);
    // Set query params depending on filters.
    switch ($values[self::DATE_TYPE]) {
      case self::DATE_TYPE_FIXED:
        $query->set(self::FIXED_RANGE, $values[self::FIXED_RANGE]);
        $query->set(self::DATE_TYPE, $values[self::DATE_TYPE]);
        break;

      case self::DATE_TYPE_CUSTOM:
        $query->set(self::DATE_TYPE, $values[self::DATE_TYPE]);
        $query->set(self::FROM, $values[self::FROM]);
        $query->set(self::TO, $values[self::TO]);
        break;
    }
  }

}
