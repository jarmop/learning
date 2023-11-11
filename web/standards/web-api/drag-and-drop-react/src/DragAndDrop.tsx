import React, { useEffect, useRef } from 'react'
import './DragAndDrop.css'

function dragstartHandler(ev: any) {
  // Add the target element's id to the data transfer object
  console.log('dragstartHandler')
  console.log(ev)
  console.log(ev.target.id)
  ev.dataTransfer.setData('text/plain', ev.target.id)
}

function dragHandler(ev: any) {
  console.log('dragHandler')
  console.log(ev)
}

export function DragAndDrop() {
  // function handleDrag(event: React.DragEvent) {
  //   console.log('handleDrag')
  //   console.log(event.clientX, event.clientY)
  // }

  const myRef = useRef<HTMLDivElement>(null)
  const linkRef = useRef<HTMLAnchorElement>(null)

  useEffect(() => {
    console.log(myRef.current)
    const myRefValue = myRef.current
    const linkRefValue = linkRef.current
    myRefValue?.addEventListener('dragstart', dragstartHandler)
    myRefValue?.addEventListener('drag', dragHandler)
    linkRefValue?.addEventListener('dragstart', dragstartHandler)
    linkRefValue?.addEventListener('drag', dragHandler)

    return () => {
      myRefValue?.removeEventListener('dragstart', dragstartHandler)
      myRefValue?.removeEventListener('drag', dragHandler)
      linkRefValue?.removeEventListener('dragstart', dragstartHandler)
      linkRefValue?.removeEventListener('drag', dragHandler)
    }
  }, [])

  return (
    <div>
      {/* <div className="draggable" onDrag={handleDrag}></div> */}
      <div className="draggable" id="div" ref={myRef}></div>
      <div className="droppable"></div>
      <a href="google.com" id="link" ref={linkRef}>
        foo
      </a>
    </div>
  )
}
