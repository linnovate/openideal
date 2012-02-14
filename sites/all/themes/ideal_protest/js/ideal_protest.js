/**
 * Implementation of Drupal behavior.
 */
Drupal.behaviors.ideal_protest = function(context) {
  $('.regular-login').hide();
	$('.login-link').click(function(){
		$('.regular-login').show();
	});
	//
	$('#mission , .view-brand-page-description .views-field-description').expander({
		slicePoint: 300,
		expandText: 'קרא עוד',
		userCollapseText: '[צמצם]'});
	//
	
};
