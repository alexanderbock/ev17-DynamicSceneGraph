let dragFunction = null;

export default {
  start: (fn) => {
    dragFunction = fn;
  },
  end: () => {
    dragFunction = null;
  }
}

document.addEventListener('mousemove', (evt) => {
  if (dragFunction != null) {
    dragFunction(evt.movementX, evt.movementY);
  }
});

document.addEventListener('mouseup', (evt) => {
  dragFunction = null;
});
