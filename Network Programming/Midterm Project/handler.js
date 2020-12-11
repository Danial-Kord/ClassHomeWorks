var maxfontSize = 30;
var minfontsize = 24;


function getData() {
    window.alert("fetching");
    fetch("https://swapi.dev/api/people/1/")
        .then((resp) => resp.json())
        .then(
            (data) =>
            (document.getElementById(
                "title"
            ).innerHTML = JSON.stringify(data))
        );
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
i = 0;
while (i < ar.length) {
    ar[i].onclick = () => {
        getData();
    }
}