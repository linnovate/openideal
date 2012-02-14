
function heartbeat_message_type_onchange(element) {
    
  if ($(element).val() == 'summary') { 
    $('#type-summary-wrapper').show();  
  }
  // Single usage
  else {
    $('#type-summary-wrapper').hide();
  }  
}

$(document).ready(function() {
  heartbeat_message_type_onchange($('#heartbeat_message_type'));
});