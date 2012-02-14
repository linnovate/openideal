<?php
// $Id: faq-questions-inline.tpl.php,v 1.1.2.5 2008/12/21 18:58:26 snpower Exp $

/**
 * @file
 * Template file for the FAQ page if set to show the questions inline.
 */

/**
 * Available variables:
 *
 * $nodes
 *   The array of nodes to be displayed.
 *   Each node stored in the $nodes array has the following information:
 *     $node['question'] is the question text.
 *     $node['body'] is the answer text.
 *     $node['links'] represents the node links, e.g. "Read more".
 * $question_label
 *   The question label, intended to be pre-pended to the question text.
 * $answer_label
 *   The answer label, intended to be pre-pended to the answer text.
 * $use_teaser
 *   Tells whether $node['body'] contains the full body or just the teaser
 */
?>
<a name="top"></a>
<div>
<?php if (count($nodes)): ?>
  <?php foreach ($nodes as $node): ?>
    <?php // Cycle through the $nodes array so that we now have a $node variable to work with. ?>
    <br />
    <div class="faq-question">
    <strong>
    <?php print $question_label; ?>
    </strong>
    <?php print $node['question']; ?>
    </div> <!-- Close div: faq-question -->

    <div class="faq-answer">
    <strong>
    <?php print $answer_label; ?>
    </strong>
    <?php print $node['body']; ?>
    <?php if (isset($node['links'])): ?>
      <?php print $node['links']; ?>
    <?php endif; ?>
    </div> <!-- Close div: faq-answer -->
  <?php endforeach; ?>
<?php endif; ?>
</div> <!-- Close div -->
