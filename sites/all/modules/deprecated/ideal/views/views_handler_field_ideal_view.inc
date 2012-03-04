<?php
/**
 * Handler for showing block.
 */
class views_handler_field_ideal_view extends views_handler_field {

  function query() {
    $this->add_additional_fields();
  }

  function render($values) { //dpm($this);
    $arg_type       = $this->options['view_argument'];
    $name           = $this->options['view_name'];
    $display_id     = $this->options['view_display_id'];
    $args           = $values->$arg_type;
    $view           = views_get_view($name);
    return views_embed_view($name, $display_id, $args); //$view->preview($display_id, $args);
  }


  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => isset($this->options['label']) ? $this->options['label'] : '',
      '#description' => t('The label for this field that will be displayed to end users if the style requires it.'),
    );
    $form['exclude'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude from display'),
      '#default_value' => $this->options['exclude'],
      '#description' => t('Check this box to not display this field, but still load it in the view.  Use this option to not show a grouping field in each record, or when doing advanced theming.'),
    );

    if ($this->allow_advanced_render()) {
      $form['alter']['#tree'] = TRUE;
      $form['alter']['alter_text'] = array(
        '#type' => 'checkbox',
        '#title' => t('Rewrite the output of this field'),
        '#description' => t('If checked, you can alter the output of this field by specifying a string of text with replacement tokens that can use any existing field output.'),
        '#default_value' => $this->options['alter']['alter_text'],
      );

      $form['alter']['text'] = array(
        '#title' => t('Text'),
        '#type' => 'textarea',
        '#default_value' => $this->options['alter']['text'],
        '#description' => t('The text to display for this field. You may include HTML. You may enter data from this view as per the "Replacement patterns" below.'),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-alter-text' => array(1),
        ),
      );

      $form['alter']['make_link'] = array(
        '#type' => 'checkbox',
        '#title' => t('Output this field as a link'),
        '#description' => t('If checked, this field will be made into a link. The destination must be given below.'),
        '#default_value' => $this->options['alter']['make_link'],
      );
      $form['alter']['path'] = array(
        '#title' => t('Link path'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['path'],
        '#description' => t('The Drupal path or absolute URL for this link. You may enter data from this view as per the "Replacement patterns" below.'),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
        ),
        '#maxlength' => 255,
      );
      $form['alter']['link_class'] = array(
        '#title' => t('Link class'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['link_class'],
        '#description' => t('The CSS class to apply to the link.'),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
        ),
      );
      $form['alter']['alt'] = array(
        '#title' => t('Alt text'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['alt'],
        '#description' => t('Text to place as "alt" text which most browsers display as a tooltip when hovering over the link.'),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
        ),
      );
      $form['alter']['prefix'] = array(
        '#title' => t('Prefix text'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['prefix'],
        '#description' => t('Any text to display before this link. You may include HTML.'),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
        ),
      );
      $form['alter']['suffix'] = array(
        '#title' => t('Suffix text'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['suffix'],
        '#description' => t('Any text to display after this link. You may include HTML.'),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
        ),
      );
      $form['alter']['target'] = array(
        '#title' => t('Target'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['target'],
        '#description' => t("Target of the link, such as _blank, _parent or an iframe's name. This field is rarely used."),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
        ),
      );


      // Get a list of the available fields and arguments for token replacement.
      $options = array();
      foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
        $options[t('Fields')]["[$field]"] = $handler->ui_name();
        // We only use fields up to (and including) this one.
        if ($field == $this->options['id']) {
          break;
        }
      }
      $count = 0; // This lets us prepare the key as we want it printed.
      foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
        $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
        $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
      }

      $this->document_self_tokens($options[t('Fields')]);

      // Default text.
      $output = t('<p>You must add some additional fields to this display before using this field. These fields may be marked as <em>Exclude from display</em> if you prefer. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
      // We have some options, so make a list.
      if (!empty($options)) {
        $output = t('<p>The following substitution patterns are available for this display. Use the pattern shown on the left to display the value indicated on the right. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
        foreach (array_keys($options) as $type) {
          if (!empty($options[$type])) {
            $items = array();
            foreach ($options[$type] as $key => $value) {
              $items[] = $key . ' == ' . $value;
            }
            $output .= theme('item_list', array('items' => $items, 'title' => $type));
          }
        }
      }
      // This construct uses 'hidden' and not markup because process doesn't
      // run. It also has an extra div because the dependency wants to hide
      // the parent in situations like this, so we need a second div to
      // make this work.
      $form['alter']['help'] = array(
        '#type' => 'hidden',
        '#id' => 'views-tokens-help',
        '#prefix' => '<div><fieldset id="views-tokens-help"><legend>' . t('Replacement patterns') . '</legend>' . $output . '</fieldset></div>',
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-make-link' => array(1),
          'edit-options-alter-alter-text' => array(1),
        ),
      );

      $form['alter']['trim'] = array(
        '#type' => 'checkbox',
        '#title' => t('Trim this field to a maximum length'),
        '#description' => t('If checked, this field be trimmed to a maximum length in characters.'),
        '#default_value' => $this->options['alter']['trim'],
      );

      $form['alter']['max_length'] = array(
        '#title' => t('Maximum length'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alter']['max_length'],
        '#description' => t('The maximum number of characters this field can be.'),
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1),
        ),
      );

      $form['alter']['word_boundary'] = array(
        '#type' => 'checkbox',
        '#title' => t('Trim only on a word boundary'),
        '#description' => t('If checked, this field be trimmed only on a word boundary. This is guaranteed to be the maximum characters stated or less. If there are no word boundaries this could trim a field to nothing.'),
        '#default_value' => $this->options['alter']['word_boundary'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1),
        ),
      );

      $form['alter']['ellipsis'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add an ellipsis'),
        '#description' => t('If checked, a "..." will be added if a field was trimmed.'),
        '#default_value' => $this->options['alter']['ellipsis'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1),
        ),
      );

      $form['alter']['html'] = array(
        '#type' => 'checkbox',
        '#title' => t('Field can contain HTML'),
        '#description' => t('If checked, HTML corrector will be run to ensure tags are properly closed after trimming.'),
        '#default_value' => $this->options['alter']['html'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array(
          'edit-options-alter-trim' => array(1),
        ),
      );

      $form['alter']['strip_tags'] = array(
        '#type' => 'checkbox',
        '#title' => t('Strip HTML tags'),
        '#description' => t('If checked, all HTML tags will be stripped.'),
        '#default_value' => $this->options['alter']['strip_tags'],
        '#process' => array('ctools_dependent_process'),
      );
    }

    $form['empty'] = array(
      '#type' => 'textfield',
      '#title' => t('Empty text'),
      '#default_value' => $this->options['empty'],
      '#description' => t('If the field is empty, display this text instead.'),
    );

    $form['empty_zero'] = array(
      '#type' => 'checkbox',
      '#title' => t('Count the number 0 as empty'),
      '#default_value' => $this->options['empty_zero'],
      '#description' => t('If the field contains the number zero, display the empty text instead'),
    );

    $form['hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide if empty'),
      '#default_value' => $this->options['hide_empty'],
      '#description' => t('Do not display anything for this field if it is empty. Note that the field label may still be displayed. Check style or row style settings to hide labels for empty fields.'),
    );

    $views = views_get_all_views();
    foreach ($views as $view) {
      if (!$view->disabled) {
        $all_views[$view->name] = $view->name;
      }
    }
    $form['view_name'] = array(
      '#type' => 'select',
      '#title' => t('The name of the view'),
      '#default_value' => $this->options['view_name'],
      '#options' => $all_views,
      '#description' => t('The view name to add.'),
    );

    $form['view_display_id'] = array(
      '#type' => 'textfield',
      '#title' => t('The display ID'),
      '#default_value' => $this->options['view_display_id'],
      '#description' => t('The view display ID (like default, page_1).'),
      '#dependency' => array(
        'edit-options-aview_name' => array(1),
      ),
    );

    $view_results = views_get_view_result($this->view->name);
    foreach ($view_results[0] as $key => $value) {
      $fields[$key] = $key;
    }

    $form['view_argument'] = array(
      '#type' => 'select',
      '#title' => t('The argument'),
      '#default_value' => $this->options['view_argument'],
      '#options' => $fields,
      '#required' => FALSE,
    );
  }
}
