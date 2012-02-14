<?php
// $Id: faq-category-questions-top.tpl.php,v 1.1.2.6 2009/02/08 17:24:49 snpower Exp $

/**
 * @file
 * Template file for the questions section of the FAQ page if set to show
 * categorized questions at the top of the page.
 */

/**
 * Available variables:
 *
 * $display_header
 *   Boolean value controlling whether a header should be displayed.
 * $header_title
 *   The category title.
 * $category_name
 *   The name of the category.
 * $answer_category_name
 *   Whether the category name should be displayed with the answers.
 * $group_questions_top
 *   Whether the questions and answers should be grouped together.
 * $category_depth
 *   The term or category depth.
 * $description
 *   The current page's description.
 * $term_image
 *   The HTML for the category image. This is empty if the taxonomy image module
 *   is not enabled or there is no image associated with the term.
 * $display_faq_count
 *   Boolean value controlling whether or not the number of faqs in a category
 *   should be displayed.
 * $question_count
 *   The number of questions in category.
 * $nodes
 *   An array of nodes to be displayed.
 *   Each node stored in the $nodes array has the following information:
 *     $node['question'] is the question text.
 *     $node['body'] is the answer text.
 *     $node['links'] represents the node links, e.g. "Read more".
 * $use_teaser
 *   Whether $node['body'] contains the full body or just the teaser text.
 * $container_class
 *   The class attribute of the element containing the sub-categories, either
 *   'faq-qa' or 'faq-qa-hide'. This is used by javascript to open/hide
 *   a category's faqs.
 * $question_list
 *   An array of question links.
 * $question_list_style
 *   The style of the question list, ul for unordered, ol for ordered.
 * $subcat_list
 *   An array of sub-categories.  Each sub-category stored in the $subcat_list
 *   array has the following information:
 *     $subcat['link'] is the link to the sub-category.
 *     $subcat['description'] is the sub-category description.
 *     $subcat['count'] is the number of questions in the sub-category.
 *     $subcat['term_image'] is the sub-category (taxonomy) image.
 * $subcat_list_style
 *   The style of the sub-category list, either ol or ul (ordered or unordered).
 * $subcat_body_list
 *   The sub-categories faqs, recursively themed (by this template).
 */

if ($category_depth > 0) {
  $hdr = 'h6';
}
else {
  $hdr = 'h5';
}

?>
<a name="top"></a>
<div class="faq-category-menu">

  <!-- category header with title, link, image, description, and count of questions inside -->
  <div class="faq-qa-header">
  <?php if ($display_header): ?>
    <<?php print $hdr; ?> class="faq-header">
    <?php print $term_image; ?>
    <?php print $header_title; ?>
    <?php if ($display_faq_count): ?>
      (<?php print $question_count; ?>)
    <?php endif; ?>
    </<?php print $hdr; ?>>
  <?php else: ?>
    <?php print $term_image; ?>
  <?php endif; ?>
  <?php if (!empty($description)): ?>
    <div class="faq-qa-description"><?php print $description ?></div>
  <?php endif; ?>
  <?php if (!empty($term_image)): ?>
    <div class="clear-block"></div>
  <?php endif; ?>
  </div> <!-- Close div: faq-qa-header -->


  <?php if (!empty($subcat_list)): ?>
    <!-- list subcategories, with title, link, description, count -->
    <div class="item-list">
    <<?php print $subcat_list_style; ?> class="faq-category-list">
    <?php foreach ($subcat_list as $i => $subcat): ?>
      <li>
      <?php print $subcat['link']; ?>
      <?php if ($display_faq_count): ?>
        (<?php print $subcat['count']; ?>)
      <?php endif; ?>
      <?php if (!empty($subcat['description'])): ?>
      <div class="faq-qa-description"><?php print $subcat['description']; ?></div>
      <?php endif; ?>
      <div class="clear-block"></div>
      </li>
    <?php endforeach; ?>
    </<?php print $subcat_list_style; ?>>
  </div> <!-- Close div: item-list -->
  <?php endif; ?>

  <div class="<?php print $container_class; ?>">

  <?php // include subcategories ?>
  <?php if (count($subcat_body_list)): ?>
    <?php foreach ($subcat_body_list as $i => $subcat_html): ?>
      <div class="faq-category-indent"><?php print $subcat_html; ?></div>
    <?php endforeach; ?>
  <?php endif; ?>

  <?php // list question links ?>
  <?php if (!empty($question_list)): ?>
    <div class="item-list">
    <<?php print $question_list_style; ?> class="faq-ul-questions-top">
    <?php foreach ($question_list as $i => $question_link): ?>
      <li>
      <?php print $question_link; ?>
      </li>
    <?php endforeach; ?>
    </<?php print $question_list_style; ?>>
  </div> <!-- Close div: item-list -->
  <?php endif; ?>

  <?php if (!$group_questions_top && $category_display != 'hide_qa'): ?>
    </div> <!-- Close div: faq-qa / faq-qa-hide -->
    </div> <!-- Close div: faq-category-menu -->
  <?php endif; ?>

  <?php if ($answer_category_name): ?>
    <!-- Display header before answers in some layouts. -->
    <<?php print $hdr; ?> class="faq-header">
    <?php print $term_image; ?>
    <?php print $category_name; ?>
    </<?php print $hdr; ?>>
    <div class="clear-block"></div>
  <?php endif; ?>

  <?php // List questions (in title link) and answers (in body). ?>
  <div class="faq-category-group">
  <div>

  <?php if (count($nodes)): ?>
    <?php foreach ($nodes as $i => $node): ?>

      <div class="faq-question"><?php // Strong question label here? ?>
      <?php print $node['question']; ?>
      </div> <!-- Close div: faq-question -->

      <div class="faq-answer">
      <strong><?php print $answer_label; ?></strong>
      <?php print $node['body']; ?>
      <?php if (isset($node['links'])): ?>
        <?php print $node['links']; ?>
      <?php endif; ?>
      </div> <!-- Close div: faq-answer -->
    <?php endforeach; ?>
  <?php endif; ?>

  </div> <!-- Close div -->
</div> <!-- Close div: faq-category-group -->

<?php if ($group_questions_top || $category_display == 'hide_qa'): ?>
</div> <!-- Close div: faq-qa / faq-qa-hide -->
</div> <!-- Close div: faq-category-menu -->
<?php
endif;
