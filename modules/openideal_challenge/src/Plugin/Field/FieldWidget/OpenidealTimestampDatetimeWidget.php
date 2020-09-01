<?php

namespace Drupal\openideal_challenge\Plugin\Field\FieldWidget;

use Drupal\Core\Datetime\DrupalDateTime;
use Drupal\Core\Datetime\Element\Datetime;
use Drupal\Core\Datetime\Entity\DateFormat;
use Drupal\Core\Datetime\Plugin\Field\FieldWidget\TimestampDatetimeWidget;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Plugin implementation of the OpenideaL 'datetime timestamp' widget.
 *
 * @FieldWidget(
 *   id = "openideal_datetime_timestamp",
 *   label = @Translation("Openideal Datetime Timestamp"),
 *   field_types = {
 *     "timestamp"
 *   }
 * )
 */
class OpenidealTimestampDatetimeWidget extends TimestampDatetimeWidget {

  /**
   * {@inheritDoc}
   */
  public function formElement(FieldItemListInterface $items, $delta, array $element, array &$form, FormStateInterface $form_state) {
    $element = parent::formElement($items, $delta, $element, $form, $form_state);
    $date_format = DateFormat::load('html_date')->getPattern();
    $time_format = DateFormat::load('html_time')->getPattern();
    $element['value']['#description'] = $this->t('<div>Note that scheduling is triggered using the server time.</div><div>Current server time is: %format.</div>', ['%format' => Datetime::formatExample($date_format . ' ' . $time_format)]);
    $element['#suffix'] = '<div class="challenge-schedule-local-machine-time"></div>';
    $element['#attached']['library'][] = 'openideal_challenge/openideal_challenge.schedule';
    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function massageFormValues(array $values, array $form, FormStateInterface $form_state) {
    foreach ($values as &$item) {
      if (isset($item['value']) && $item['value'] instanceof DrupalDateTime) {
        $date = $item['value'];
      }
      elseif (isset($item['value']['object']) && $item['value']['object'] instanceof DrupalDateTime) {
        $date = $item['value']['object'];
      }

      $item['value'] = !empty($date) ? $date->getTimestamp() : NULL;
    }
    return $values;
  }

}
