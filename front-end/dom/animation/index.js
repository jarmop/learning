let element = document.getElementById('element');

let frameCount = 0;
const animate = (maxFrames) => {
    element.style.height = element.clientHeight + 1 + 'px' ;
    frameCount++;
    console.log(frameCount);

    if (frameCount < maxFrames) {
        console.log('requestAnimationFrame');
        window.requestAnimationFrame(() => animate(maxFrames));
    }
};

animate(100);

