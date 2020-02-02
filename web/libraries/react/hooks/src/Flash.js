import React, { useState, useEffect } from 'react'
import './Flash.css'

const Flash = () => {
    const [flashIndex, setFlashIndex] = useState(-1)
    const [flashEnabled, setFlashEnabled] = useState(false)
    const [something, setSomething] = useState('wrtwr')

    useEffect(() => {
        console.log('flashIndex: ' + flashIndex)
        setFlashEnabled(true)
    })

    const doFlash = (index) => {
        setFlashIndex(index)
        setFlashEnabled(false)
        // setTimeout(() => {
        //     setFlash(true)
        // }, 1)
    }

    const doNotFlash = () => {
        console.log('do not flash')
        setSomething(Math.random() * 100)
    }

    return (
        <div className="Flash">
            <div>
                <button onClick={() => doFlash(0)}>
                    Flash first
                </button>
                <button onClick={() => doFlash(1)}>
                    Flash second
                </button>
                <button onClick={() => doNotFlash()}>
                    Do not flash
                </button>
            </div>
            {something}
            <Flasher flash={flashEnabled && flashIndex === 0} />
            <Flasher flash={flashEnabled && flashIndex === 1} />
        </div>
    )
}

export default Flash

const Flasher = ({ flash = false }) => {
    return (
        <div className={'Flasher' + (flash ? ' Flasher--flashing' : '')}>
            {flash ? 'flashing' : 'not flashing'}
        </div>
    )
}