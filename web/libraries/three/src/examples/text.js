import * as THREE from 'three'

import { FontLoader } from 'three/addons/loaders/FontLoader.js'
import { TextGeometry } from 'three/addons/geometries/TextGeometry.js'

THREE.Cache.enabled = true

function main() {
  const scene = new THREE.Scene()

  const loader = new FontLoader()
  loader.load('optimer_bold.typeface.json', function (response) {
    const textGeo = new TextGeometry('text', {
      font: response,
      size: 70,
      depth: 20,
      curveSegments: 4,
      bevelThickness: 2,
      bevelSize: 1.5,
      bevelEnabled: true,
    })
    const textMesh1 = new THREE.Mesh(textGeo)
    scene.add(textMesh1)
  })

  const camera = new THREE.PerspectiveCamera(30, window.innerWidth / window.innerHeight, 1, 2000)
  camera.position.set(0, 0, 2000)
  const cameraTarget = new THREE.Vector3(0, 0, 0)
  const renderer = new THREE.WebGLRenderer({ antialias: true })
  renderer.setAnimationLoop(function animate() {
    camera.lookAt(cameraTarget)

    renderer.clear()
    renderer.render(scene, camera)
  })

  document.body.appendChild(renderer.domElement)
}

export { main }
