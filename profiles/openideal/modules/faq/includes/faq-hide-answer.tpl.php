<?php

/**
 * @file
 * Template file for the FAQ page if set to show/hide the answers when the
 * question is clicked.
 */

/**
 * Available variables:
 *
 * $nodes
 *   The array of nodes to be displayed.
 *   Each $node array contains the following information:
 *     $node['question'] is the question text.
 *     $node['body'] is the answer text.
 *     $node['links'] represents the node links, e.g. "Read more".
 * $use_teaser
 *   Is true if $node['body'] is a teaser.
 */
?><div>
<?php if (count($nodes)): ?>
  <?php foreach ($nodes as $node): ?>
    <?php // Cycle through each of the nodes. We now have the variable $node to work with. ?>
    <div class="faq-question-answer">
    <div class="faq-question faq-dt-hide-answer">
    <?php print $node['question']; ?>
    </div> <!-- Close div: faq-question faq-dt-hide-answer -->

    <div class="faq-answer faq-dd-hide-answer">
    <?php print $node['body']; ?>
    <?php if (isset($node['links'])): ?>
      <?php print $node['links']; ?>
    <?php endif; ?>
    </div> <!-- Close div: faq-answer faq-dd-hide-answer -->
    </div> <!-- Close div: faq-question-answer -->
  <?php endforeach; ?>
<?php endif; ?>
</div> <!-- Close div -->
