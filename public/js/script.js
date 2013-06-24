window.onload = function() {

	/* ip address of arduino */
	var URL = "00.0.0.0";
	var lightDiv = $('#lightbulbContainer');

	/* find out current state of the light and adjust the UI accordingly */
	$.ajax({
		type: "GET",
		url: URL+"/led=-1"
	}).done(function(response) {
		lightObj = $.parseJSON(''+response+'');
		lightDiv.addClass('light'+lightObj.status);
	});

	/* click event to change light */
	lightDiv.bind('click', changeLightDiv = function () {
		var status = lightDiv.hasClass('lighton') ? 'on' : 'off';
		if(status=='on') {
			lightDiv[0].className='lightoff';
			changeLightBulb('0');
		} else {
			lightDiv[0].className='lighton';
			changeLightBulb('1');
		}

	});

	/* tell the light to turn on or off */
	function changeLightBulb(status) {
		$.ajax({
			type: "GET",
			url: URL+"/led="+status
		}).done(function(response) {
			responseObj = $.parseJSON(''+response+'');
		});
	}


}