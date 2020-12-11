var maxfontSize = 30;
var minfontsize = 24;
// parsing json data elemnts for presenting in result box of the page
function calculateData(data) {
    i = 1;
    data = data["results"][0]
        //remove old data from the page for new coming data
    dadElemnt = document.getElementById("result").children[0];
    for (let j = 1; j < dadElemnt.children.length;) {
        dadElemnt.children[1].remove();
    }
    /*
    finding every key value target in the JSON responce and showing it
    with the creation of the new elemnt and appending it to page
    ps: upto 14 elemnts are enough! the rest are not important data
    */
    let text = ""
    for (x in data) {
        text = x + " : " + data[x];
        let newElement = document.createElement("dd");
        newElement.innerHTML = text;
        dadElemnt.appendChild(newElement);
        i++;
        if (i == 14) {
            break;
        }
    }
}

// fetching data from the api and calculationg responce data
function getData(index) {
    window.alert("fetching");
    fetch("https://swapi.dev/api/starships/?page=1")
        .then((resp) => resp.json())
        .then((data) => calculateData(data))
}


/*
setting up cursur change shape considering the location of the cursur
with mouse events for better client experience
*/
let tableData = document.getElementsByTagName("td");
let i = 0;
while (i < tableData.length) {
    tableData[i].onmouseenter = () => {
        document.body.style.cursor = "pointer";
        tableData[i].style.fontSize = maxfontSize;
    }

    tableData[i].onmouseleave = () => {
        document.body.style.cursor = "auto";
        tableData[i].style.fontSize = minfontsize;
    }
    i++;
}

// handling on click event
let tableClick = document.getElementsByTagName("td");
let h = 0;
while (h < tableClick.length) {
    tableClick[h].onclick = () => {
        getData();
    }
    h++;
}