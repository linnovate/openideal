<?php

namespace Drupal\openideal_statistics;

use Drupal\node\NodeInterface;
use Drupal\openideal_idea\OpenidealHelper;

/**
 * Helps to build votingapi fields.
 */
trait OpenidealStatisticsFivestarsTrait {

  /**
   * Build fivestars view.
   *
   * @param \Drupal\node\NodeInterface $idea
   *   Idea.
   *
   * @return array
   *   Rendarable array.
   */
  private function viewFivestars(NodeInterface $idea) {
    $fivestars = [];
    $defaultSettings = [
      'label' => 'inline',
      'settings' => [
        'show_results' => '1',
        'style' => 'fontawesome-stars',
      ],
    ];

    $fields = $idea->getFieldDefinitions();
    foreach ($fields as $field_name => $field_definition) {
      if (OpenidealHelper::isVotingAPIField($field_definition)) {
        $fivestars[$field_name] = $idea->get($field_name)->view($defaultSettings);
      }
    }

    return $fivestars;
  }

}
