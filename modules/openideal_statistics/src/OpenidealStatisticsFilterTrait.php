<?php

namespace Drupal\openideal_statistics;

use Drupal\openideal_statistics\Form\OpenidealStatisticsDateSelectForm as Filter;

/**
 * Provide help method to indicate filter for charts family blocks.
 *
 * @see openideal_charts
 *
 * @package Drupal\openideal_statistics
 */
trait OpenidealStatisticsFilterTrait {

  /**
   * Get filters.
   *
   * @return array|false
   *   Filters to apply.
   */
  public function getFilters() {
    // @Todo: get request service in block and pass here?
    $query = \Drupal::request()->query;
    if ($query->has(Filter::FIXED_RANGE)) {
      $range = $query->get(Filter::FIXED_RANGE);
      return [Filter::FROM => strtotime('-' . $range . ' month')];
    }
    elseif ($query->has(Filter::FROM) && $query->has(Filter::TO)) {
      return [
        Filter::FROM => strtotime($query->get(Filter::FROM)),
        Filter::TO => strtotime($query->get(Filter::TO)),
      ];
    }

    return FALSE;
  }

}
