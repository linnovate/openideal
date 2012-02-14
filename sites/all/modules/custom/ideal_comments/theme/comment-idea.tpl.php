<?php
// $Id: comment.tpl.php,v 1.4.2.1 2008/03/21 21:58:28 goba Exp $

/**
 * @file comment.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
dpm($comment);
?>
<?php global $user;?>

<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' ' . $status . ' ' . $author_ideal?> clear-block">
	<?php print $picture;?>
  <div class="submitted">
    <div class="comment-author"><?php print $author ?></div>
    <div class="comment-time"><?php print $date ?></div>
  </div>

  <div class="content-wrapper">
    <div class="content-inner">
    
      <?php if ($comment->new): ?>
        <span class="new"><?php print $new ?></span>
      <?php endif; ?>

      <?php print $content ?>
      
      <?php print $links ?>

      <?php 
        print drupal_get_form("comment_form_$node->nid$comment->cid",array('nid' => $node->nid, 'pid' => $comment->cid));      ?>

    </div>
  </div>
</div>
