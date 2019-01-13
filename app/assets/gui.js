function setButtonText(text) {
  document.getElementById("button").innerHTML = text
}

// JS Bindings
document.querySelector('#button').addEventListener('click', () => onButtonClick())
