// $Id: fivestar-admin.js,v 1.1.4.4 2009/05/10 20:56:24 quicksketch Exp $

/**
 * Fivestar admin interface enhancments.
 */

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    var nodePreview = new fivestarPreview($('#fivestar-direct-preview .fivestar-preview')[0]);

    // Hide extra mouseover textfields
    nodePreview.displayTextfields();

    // Enable comments if available.
    $comment = $('input[name=fivestar_comment]');
    if ($comment.size()) {
      var commentPreview = new fivestarPreview($('#fivestar-comment-preview .fivestar-preview')[0]);
    }

    // Setup dynamic form elements.
    $enable = $('#edit-fivestar');
    $unvote = $('#edit-fivestar-unvote');
    $title  = $('#edit-fivestar-title');
    $feedback = $('#edit-fivestar-feedback');
    $style  = $('#edit-fivestar-style');
    $text   = $('#edit-fivestar-text');

    // All the form elements except the enable checkbox.
    $options = $('#fivestar-node-type-form input:not(#edit-fivestar), #fivestar-node-type-form select');

    // Disable the settings if not enabled.
    if (!$enable.attr('checked')) {
      $options.attr('disabled', 'disabled');
      nodePreview.disable();
    }
    else {
      nodePreview.enable($unvote.attr('checked') ? 1 : 0, $style.val(), $text.val(), $title.attr('checked') ? 1 : 0, $feedback.attr('checked') ? 1 : 0);
    }

    // Add event handler for enable checkbox.
    $enable.change(function() {
      if ($(this).attr('checked')) {
        // Enable the node preview.
        nodePreview.enable($unvote.attr('checked') ? 1 : 0, $style.val(), $text.val(), $title.attr('checked') ? 1 : 0, $feedback.attr('checked') ? 1 : 0);
        nodePreview.update()

        // Enable the comment preview if available.
        if (commentPreview) {
          var commentSetting = 0;
          $comment.each(function() {
            if ($(this).attr('checked')) {
              commentSetting = this.value;
            }
          });
          if (commentSetting != 0) {
            commentPreview.enable(commentSetting == 1 ? 1 : 0, 'user', 'none', 0, 0);
            commentPreview.update();
          }
        }
        $options.attr('disabled', false);
      }
      else {
        nodePreview.disable();
        if (commentPreview) {
          commentPreview.disable();
        }
        $options.attr('disabled', 'disabled');
      }
    });

    // Setup node specific preview handlers.
    $style.change(function() { nodePreview.setValue('style', this.value); });
    $text.change(function() { nodePreview.setValue('text', this.value); });
    $title.change(function() { nodePreview.setValue('title', $(this).attr('checked') ? 1 : 0); });
    $unvote.change(function() { nodePreview.setValue('unvote', $(this).attr('checked') ? 1 : 0); });
    $feedback.change(function() { nodePreview.setValue('feedback', $(this).attr('checked') ? 1 : 0); });

    if (commentPreview) {
      // Enable the comment preview.
      if ($enable.attr('checked')) {
        commentPreview.enable(this.value == 1 ? 1 : 0, 'user', 'none', 0, 0);
      }
      else {
        commentPreview.disable();
      }

      // Setup comment preview handlers.
      $comment.change(function() {
        if ($(this).attr('checked') && $enable.attr('checked')) {
          if (this.value != 0) {
            commentPreview.setValue('unvote', this.value == 1 ? 1 : 0);
            commentPreview.enable(this.value == 1 ? 1 : 0, 'user', 'none', 0, 0);
            commentPreview.update();
          }
          else {
            commentPreview.disable();
          }
        }
      });
    }
  });
}

/**
 * Constructor for fivestarPreview.
 * @param previewId
 *   The id attribute of the div containing the preview.
 */
var fivestarPreview = function(previewElement) {
  // Elements that need handlers.
  this.elements = new Object();
  this.elements.stars  = $('#edit-fivestar-stars');
  this.elements.labels = $('.fivestar-label input');
  this.elements.labelsEnable = $('#edit-fivestar-labels-enable');

  // Private variables.
  this.preview = previewElement;
  this.enabled = false;
  this.unvote = 0;
  this.title = 1;
  this.feedback = 1;
  this.stars = this.elements.stars.val();
  this.style = '';
  this.text = '';
  this.labels = new Object();
  this.labelsEnable = false;

  // Setup handlers that affect all previews.
  var self = this;
  this.elements.stars.change(function() { self.setValue('stars', this.value); self.displayTextfields(); });
  this.elements.labelsEnable.change(function() { self.setValue('labelsEnable', $(this).attr('checked') ? 1 : 0); });

  // Handler for the star labels.
  var currentLabel = '';
  this.elements.labels.focus(function() {
    currentLabel = this.value;
  });
  this.elements.labels.blur(function() {
    if (currentLabel != this.value) {
      self.setLabel(self.elements.labels.index(this), this.value);
    }
  });
};

/**
 * Enable the preview functionality and show the preview.
 */
fivestarPreview.prototype.enable = function(unvote, style, text, title, feedback) {
  if (!this.enabled) {
    this.enabled = true;

    // Update global settings.
    this.stars = this.elements.stars.val();
    var labels = new Array();
    this.elements.labels.each(function(n) {
      labels[n] = this.value;
    });
    this.labels = labels;
    this.labelsEnable = this.elements.labelsEnable.attr('checked') ? 1 : 0;

    // Update settings specific to this preview.
    this.unvote = unvote;
    this.title = title;
    this.feedback = feedback;
    this.style = style;
    this.text = text;

    // Show the preview.
    $(this.preview).css('display', 'block');
  }
};

/**
 * Disable the preview functionality and show the preview.
 */
fivestarPreview.prototype.disable = function() {
  this.enabled = false;
  $(this.preview).css('display', 'none');
};

fivestarPreview.prototype.setValue = function(field, value) {
  if (this[field] != value) {
    this[field] = value;
    if (this.enabled) {
      this.update();
    }
  }
};

fivestarPreview.prototype.setLabel = function(delta, value) {
  this.labels[delta] = value;
  if (this.enabled) {
    this.update();
  }
}

fivestarPreview.prototype.update = function() {
  if (this.enabled) {
    var self = this;
    var updateSuccess = function(response) {
      // Sanity check for browser support (object expected).
      // When using iFrame uploads, responses must be returned as a string.
      if (typeof(response) == 'string') {
        response = Drupal.parseJson(response);
      }
      $(self.preview).html(response.data).hide();
      $('div.fivestar-form-item', self.preview).fivestar();
      $('input.fivestar-submit', self.preview).hide();
      $(self.preview).show();
    };

    // Prepare data to send to the server.
    var data = { style: this.style, text: this.text, stars: this.stars, unvote: this.unvote, title: this.title, feedback: this.feedback, labels_enable: this.labelsEnable };

    // Covert labels array format understood by PHP and add to data.
    for (n in this.labels) {
      data['labels['+ n +']'] = this.labels[n];
    }

    $.ajax({
      dateType: 'json',
      type: 'POST',
      url: Drupal.settings.fivestar.preview_url,
      data: data,
      success: updateSuccess
    });
  }
};

// Display the appropriate number of text fields for the mouseover star descriptions
fivestarPreview.prototype.displayTextfields = function() {
  for (var count = 0; count <= 10; count++) {
    if (count <= this.stars) {
      $('#fivestar-label-'+ count).show();
    }
    else {
      $('#fivestar-label-'+count).css('display', 'none');
    }
  }
};
