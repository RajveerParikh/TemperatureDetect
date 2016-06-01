


Pebble.addEventListener("appmessage",
  function(e) {
    //console.log("!!!!!!!!!!!\n");
    
    console.log(e.payload.hello_msg);
    if(e.payload.hello_msg=="e")
    {
      sendToServer2();}
    else
    {sendToServer(e.payload.hello_msg);}
});





function sendToServer(message) {

	var req = new XMLHttpRequest();
	var ipAddress = "158.130.108.255"; // Hard coded IP address
	var port = "3000"; // Same port specified as argument to server
	var url = "http://" + ipAddress + ":" + port + "/" + message;
	var method = "GET";
	var async = true;
  
  var check =1;
  req.timeout = 2000;
  
  req.open(method, url, async);
  req.send(null);
	req.onload = function(e) {
                // see what came back
                check=0;
    console.log("!!!!enter onload");
                var msg = "no response";
                var response = JSON.parse(req.responseText);
                if (response) {
                    if (response.name) {
                        msg = response.name;
                    }
                    else msg = "noname";
                }
//                 if (!(e.payload.error_msg)){
                   //check = 0;
//                 }
                // sends message back to pebble
                Pebble.sendAppMessage({ "0": msg });
	};
  
//   req.ontimeout = function(e){
//       console.log("enter ontimeout!!!!!!\n")
//      Pebble.sendAppMessage({ "0": "Server is down." });
//   };
  
  
  if(check == 1) {
//     Pebble.sendAppMessage({ "0": "1233453341353" });
  req.onerror = function(){
              var msg1 = "Server is down."
              //var response = JSON.parse(req.responseText);
              Pebble.sendAppMessage({ "0": msg1 });
  };
  }
  
        console.log("????????????????\n");
        
        
        
}

function sendToServer2() {

	var req = new XMLHttpRequest();
	//var ipAddress = "158.130.107.87"; // Hard coded IP address
	var port = "3000"; // Same port specified as argument to server
	var url = "http://www.pm25.in/api/querys/pm2_5.json?city=beijing&token=5j1znBVAsnSf5xQyNQyq&stations=no";
	var method = "GET";
	var async = true;
  
  
  
	req.onload = function(e) {

                // see what came back
                var msg = "no response";
                var response = JSON.parse(req.responseText);
                msg = req.responseText.split(",")[2];
                console.log(msg);
//                 if (response) {
                  
//                     if (response.pm2_5) {
//                         msg = response.pm2_5;
//                     }
//                     else msg = "noname";
//                 }
                // sends message back to pebble
                Pebble.sendAppMessage({ "0": msg });
	};
        //console.log("????????????????\n");
        req.open(method, url, async);
        
        req.send(null);
}