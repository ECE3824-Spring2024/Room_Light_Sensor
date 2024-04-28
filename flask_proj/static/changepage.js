const butt = document.getElementById("search_button")
if(butt) {

    document.getElementById("search_button").addEventListener("click",
	    function() {
	        // do stuff
	        mydate = document.getElementById("date_request").value;
	        console.log(mydate);
   	    }
    )
}
