<?php

namespace Drupal\openideal_challenge\Service;

/**
 * OpenidealChallengeServiceInterface file.
 */
interface OpenidealChallengeServiceInterface {

  /**
   * Processing for opening scheduled nodes.
   */
  public function openChallenges();

  /**
   * Processing for closing scheduled nodes.
   */
  public function closeChallenges();

  /**
   * Get the number of ideas which belongs to specific challenge.
   *
   * @param string $challenge_id
   *   Challenge id.
   *
   * @return string
   *   The count of ideas.
   */
  public function getCountOfIdeas($challenge_id);

}
