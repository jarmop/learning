import React from 'react'
import CommonComponent from 'common/CommonComponent'
import MinorComponent from './MinorComponent'

function SpecialComponent() {
  return (
    <div>
      <div>Special</div>
      <CommonComponent />
      <MinorComponent />
    </div>
  )
}

export default SpecialComponent
