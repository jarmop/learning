export function Path() {
  const width = 200
  const height = 100

  return (
    <div>
      <p>
        <code>m20 20 h50 v50 h-50</code>
      </p>
      <svg width={width} height={height}>
        <path d="m20 20 h50 v50 h-50" />
      </svg>
      <p>
        The default fill color is black. If we set fill "white" and stroke "black", we'll better see how the path
        commands are actually interpreted.
      </p>
      <p>
        <code>fill="white" stroke="black"</code>
      </p>
      <svg width={width} height={height}>
        <path fill="white" stroke="black" d="m20 20 h50 v50 h-50" />
      </svg>
      <p>
        So it fills the area inside the dots even if the dots are not connected. We can connect the dots by adding "z"
        at the end.
      </p>
      <p>
        <code>m20 20 h50 v50 h-50 z</code>
      </p>
      <svg width={width} height={height}>
        <path fill="white" stroke="black" d="m20 20 h50 v50 h-50 z" />
      </svg>
      <p>
        We could also just draw a normal line from end to start (v-50), but then the start and end would not be exactly
        connected, they would just reside at the same point. We can better see the difference if we increase the
        strokeWidth. The first box is connected with "z", the other wtih "v-50".
      </p>
      <svg width={width} height={height}>
        <path fill="white" stroke="black" strokeWidth={10} d="m20 20 h50 v50 h-50 z" />
        <path fill="white" stroke="black" strokeWidth={10} d="m100 20 h50 v50 h-50 v-50" />
      </svg>
      <p>If we want to draw lines that move in both coordinates we need to use the "l" command.</p>
      <p>
        <code>m20 60 l40 -40 l40 40</code>
      </p>
      <svg width={width} height={height}>
        <path fill="white" stroke="black" d="m20 60 l40 -40 l40 40" />
      </svg>
      <p>For any command, we can use absolute values instead of relative. This is done by using uppercase letters.</p>
      <p>
        <code>M20 60 L60 20 L100 60</code>
      </p>
      <svg width={width} height={height}>
        <path fill="white" stroke="black" d="M20 60 L60 20 L100 60" />
      </svg>
      <p>
        with the first command the relative and absolute is the same because the starting point is "0,0", but with the
        other commands we need to increment the previous coordinates.
      </p>
      <p>
        The line commands can also be specified without the "l" letter. Plain number pairs will be interpeted as line
        commands. The "relative/absolute" setting comes from the previous command.
      </p>
      <p>
        <code>m20 60 40-40 40 40</code>
      </p>
      <svg width={width} height={height}>
        <path fill="white" stroke="black" d="m20 60 40-40 40 40" />
      </svg>
      <p>Here's Australia (without Tasmania, but with New Zealand).</p>
      <svg width={width} height={height}>
        <path
          fill="white"
          stroke="black"
          d="m100 60 -1 3.2 7.3-5.2-2.1-2.8v-3.2l-1.2 0.5v4.5zm-19.2 10.5 15.2-7.7v2l-11.7 7.7zm-62.3-39.5 12.8-6 9.5-10.5 5.2 2.3 2.3-4.8 8.7-0.2-2.2 4 7.2 6.2 4.8-4.8v-6.7l2.2 0.7 2.5 12 7.8 11-0.8 8.3-14.2 14.7-12.3-0.7-1.5-8.3-4.2 1.8-4-3.5-24.3 5.3-1.5-1.8 2-4.8z"
        />
      </svg>
    </div>
  )
}
