<?php

$plugin = array(
    'single' => TRUE,
    'title' => t('idea widget pane'),
    'description' => t('Angular widget of idea add/edit form .'),
    'category' => array(t('OpenideaL'), 7),
    'all contexts' => TRUE,
);

function idea_widget_widget_block_content_type_render($subtype, $conf, $panel_args, $contexts) {
    global $language;
    $block = new stdClass();
    $block->title = '';
    
    $output = '<div><script type="text/javascript" src="/sites/all/modules/idea_widget/idea_widget.js"></script></div>';
    $block->content = $output;
    return $block;
}


