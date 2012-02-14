
/**
 * Hook into the before post of shouting.
 */
$(document).bind('heartbeatBeforePoll', function(event, post) {
  post.group_nid = Drupal.settings.group_nid;
});
