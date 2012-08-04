
function setData()
{
	
	//setInterval("setData()", 10000);
	document.getElementById("CPUName").innerHTML = cpuName;
	document.getElementById("CPUload").innerHTML = cpuLoad;
	document.getElementById("CPUSpeed").innerHTML = cpuSpeed;
	document.getElementById("GPUName").innerHTML = gpuName;
	document.getElementById("GPUUsage").innerHTML = gpuLoad;
	document.getElementById("GPUTemp").innerHTML = gpuTemp;
	document.getElementById("memory").innerHTML = memory;
	document.getElementById("hardDisk").innerHTML = hardDisk;
	
	var height = 260 + (25 *(totalHd-1));
	
	document.getElementById('bodyId').style.height = height;
	
	var positionImage = (height-180)/2;
	
	document.getElementById('logo').style.top = positionImage;

	
	setTimeout("window.location.reload(true);", 5000);
}
