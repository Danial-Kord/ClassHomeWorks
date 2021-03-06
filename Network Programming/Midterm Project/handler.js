const maxDataShow = 8;
const maxfontSize = 40;
const minfontsize = 24;
var URL = "https://swapi.dev/api/starships/?page=1"

// parsing json data elemnts for presenting in result box of the page
function calculateData(data, pageIndex) {
    i = 1;
    data = data["results"][pageIndex]
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
        newElement.className = "responce";
        newElement.style = document.styleSheets;
        newElement.innerHTML = text;
        dadElemnt.appendChild(newElement);
        i++;
        if (i == maxDataShow) {
            break;
        }
    }
    films = data["films"]
        //find films
    for (x in films) {
        fetch(films[x]).then((response) => {
                if (!response.ok) {
                    window.alert("Network response was not ok\n CODE:, " + response.status);
                    throw new Error(
                        "Network response was not ok\n CODE:, " + response.status
                    );
                }
                return response;
            }).then((resp) => resp.json())
            .then((data) => {
                text = "film : " + data["title"];
                let newElement = document.createElement("dd");
                newElement.className = "responce";
                newElement.style = document.styleSheets;
                newElement.innerHTML = text;
                dadElemnt.appendChild(newElement);
            })
            .catch();
    }

}
//check if request timeout throw error
//set a timer and if the responce comes quicker than timeout it will do the rest of tasks otherwise will return new Error
function timeout(ms, promise) {
    return new Promise(function(resolve, reject) {
        setTimeout(() => reject(new Error("timeout")), ms)
        promise.then(resolve, reject)
    })
}

// fetching data from the api and calculationg responce data and status checking
function getData(index) {
    timeout(10000, fetch(URL)).then((response) => {
            if (!response.ok) {
                window.alert("Network response was not ok\n CODE:, " + response.status);
                throw new Error(
                    "Network response was not ok\n CODE:, " + response.status
                );
            }
            return response;
        }).then((resp) => resp.json())
        .then((data) => calculateData(data, index))
        .catch(function(error) {
            window.alert("check your connection");

        })

}

var currentPageIndex = 1;
const maxPages = 4;


// setting all table row events
function startUp(array) {
    // handling on click event

    let tableClick = document.getElementsByTagName("td");
    for (let i = 0; i < tableClick.length; i++) {
        if (array.length <= i) {
            tableClick[i].innerHTML = "";
        } else {
            tableClick[i].innerHTML = array[i];
            tableClick[i].onclick = () => {
                getData(i);
            }
        }
    }
}



// when 2 buttons of the sides are clicked page table elemnts need to be refreshed for new elemnts
function fetchTableElemnts() {
    URL = "https://swapi.dev/api/starships/?page=" + currentPageIndex;
    timeout(10000, fetch(URL)).then((response) => {
            if (!response.ok) {
                window.alert("Network response was not ok\n CODE:, " + response.status);
                throw new Error(
                    "Network response was not ok\n CODE:, " + response.status
                );
            }
            return response;
        }).then((resp) => resp.json())
        .then((data) => {
            let a = [];
            for (let i = 0; i < data["results"].length; i++) {
                let temp = data["results"][i];
                a.push(temp["name"]);
            }
            startUp(a);
        })
        .catch(function(error) {
            window.alert("check your connection");

        })
}




/*
setting up cursur change shape considering the location of the cursur
with mouse events for better client experience
*/
let tableData = document.getElementsByTagName("td");



for (let i = 0; i < tableData.length; i++) {
    tableData[i].onmouseenter = () => {
        document.body.style.cursor = "pointer";
        tableData[i].style.fontSize = "" + maxfontSize;
        tableData[i].style.backgroundColor = "lightblue";
    }

    tableData[i].onmouseleave = () => {
        document.body.style.cursor = "auto";
        tableData[i].style.fontSize = "" + minfontsize;
        tableData[i].style.backgroundColor = "transparent";

    }
}

let buttons = document.getElementsByTagName("button");

buttons[1].onclick = () => {
    currentPageIndex++;
    if (currentPageIndex > maxPages) {
        currentPageIndex = maxPages;
    } else { fetchTableElemnts(); }
};

buttons[0].onclick = () => {
    currentPageIndex--;
    if (currentPageIndex < 1) {
        currentPageIndex = 1;
    } else {
        fetchTableElemnts();
    }
};

fetchTableElemnts()