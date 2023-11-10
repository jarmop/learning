function dragstartHandler(ev) {
  console.log('dragstart')
  // Add the target element's id to the data transfer object
  ev.dataTransfer.setData('text/plain', ev.target.id)
}

window.addEventListener('DOMContentLoaded', () => {
  const element = document.querySelector('.draggable')
  // Add the ondragstart event listener
  element.addEventListener('dragstart', dragstartHandler)
})
