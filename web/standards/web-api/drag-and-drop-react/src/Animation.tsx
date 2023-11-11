import React, { useEffect } from 'react'
import { animation } from './animationHelper'

function Animation() {
  useEffect(() => {
    animation()
  }, [])

  return (
    <>
      <div id="element" style={{ height: '50px', width: '50px', backgroundColor: 'royalblue' }}></div>
    </>
  )
}

export default Animation
