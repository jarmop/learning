function dragstartHandler(ev) {
  const dragElement = ev.target

  const dragRepresentation = dragElement.cloneNode(true)
  dragRepresentation.id = 'dragRepresentation'
  dragRepresentation.style.position = 'absolute'
  dragRepresentation.style.top = '-1000px'
  dragRepresentation.style.width = `${dragElement.offsetWidth}px`
  dragRepresentation.style.height = `${dragElement.offsetHeight}px`
  document.body.appendChild(dragRepresentation)

  ev.dataTransfer.effectAllowed = 'copyMove'
  ev.dataTransfer.setDragImage(dragRepresentation, ev.target.offsetWidth / 2, ev.target.offsetHeight / 2)

  dragElement.classList.add('dragged')
}

function dragendHandler(ev) {
  console.log('dragend')
  const dragElement = ev.target
  dragElement.classList.remove('dragged')
  const dragRepresentation = document.getElementById('dragRepresentation')
  document.body.removeChild(dragRepresentation)
}

function dragHandler(ev) {
  const hoverElement = document.elementFromPoint(ev.clientX, ev.clientY)
  const isDraggable = Array.prototype.includes.call(hoverElement.classList, 'draggable')
  if (!isDraggable) {
    return
  }

  const dragElement = ev.target
  if (hoverElement.textContent !== dragElement.textContent) {
    const list = document.querySelector('.list')

    const hoverBeforeDrag = hoverElement.compareDocumentPosition(dragElement) & Node.DOCUMENT_POSITION_FOLLOWING
    if (hoverBeforeDrag) {
      list.insertBefore(dragElement, hoverElement)
    } else {
      list.insertBefore(hoverElement, dragElement)
    }
  }
}

window.addEventListener('DOMContentLoaded', () => {
  const elements = document.querySelectorAll('.draggable')
  elements.forEach((node) => {
    node.addEventListener('dragstart', dragstartHandler)
    node.addEventListener('drag', dragHandler)
    node.addEventListener('dragend', dragendHandler)

    // Prevent snap back effect on drop
    node.addEventListener('dragover', (e) => e.preventDefault())
  })
})
