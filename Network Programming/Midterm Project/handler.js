var maxfontSize = 30;
var minfontsize = 24;

function calculateData(data) {

}


function getData(index) {
    window.alert("fetching");
    fetch("https://swapi.dev/api/starships/?page=1")
        .then((resp) => resp.json())
        .then(
            (data) => {
                i = 1;
                data = data["results"][0]
                dadElemnt = document.getElementById("result").children[0];

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

                document.getElementById(
                    "title"
                ).innerHTML = "hi betch ";
            })
}



let ar = document.getElementsByTagName("td");
let i = 0;
while (i < ar.length) {
    ar[i].onmouseenter = () => {
        document.body.style.cursor = "pointer";
        ar[i].style.fontSize = maxfontSize;
    }

    ar[i].onmouseleave = () => {
        document.body.style.cursor = "auto";
        ar[i].style.fontSize = minfontsize;
    }
    i++;
}


let br = document.getElementsByTagName("td");
let h = 0;
while (h < ar.length) {
    br[h].onclick = () => {
        getData();
    }
    h++;
}