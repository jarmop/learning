import React, { useState, useEffect, useRef } from 'react'
import './Flash.css'

const Flash = () => {
    const [flashIndex, setFlashIndex] = useState(-1)
    const [something, setSomething] = useState('wrtwr')

    useEffect(() => {
        if (flashIndex !== -1) {
            setFlashIndex(-1)
        }
    }, [setFlashIndex, flashIndex])

    const doFlash = (index) => {
        setFlashIndex(index)
    }

    const doNotFlash = () => {
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
            <Flasher flash={flashIndex === 0} />
            <Flasher flash={flashIndex === 1} />
        </div>
    )
}

export default Flash

const Flasher = ({ flash = false }) => {
    const ref = useRef()
    // const [domObject, setDomObject] = useState()

    // const setDomObjecti = (domo) => {
    //     console.log('set dom')
    //     setDomObject(domo)
    // }

    useEffect(() => {
        console.log('yru')
        console.log(flash)
        if (flash) {
            ref.current.classList.remove('Flasher--flashing')
            // console.log(domObject)
            // domObject.classList.remove('Flasher--flashing')
            window.requestAnimationFrame(() => {
                console.log('addFlash')
                ref.current.classList.add('Flasher--flashing')
                // domObject.classList.add('Flasher--flashing')

            })
        }
    // }, [flash, domObject])
    }, [flash])

    return (
        <div ref={ref} className="Flasher">
        {/* <div ref={setDomObjecti} className="Flasher"> */}
            {flash ? 'wrth' : 'wrtrwt'}
        </div>
    )
}