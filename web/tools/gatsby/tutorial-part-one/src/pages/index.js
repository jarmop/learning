import React from "react"
import Link from "gatsby-link";

export default () => (
  <div style={{ color: 'tomato' }}>
    <h1>Hello, Gatsby!</h1>
    <p>What a world.</p>
    <img src="https://source.unsplash.com/random/400x200" alt="" />
    <ul>
      <li>
        <Link to="/whatever-page/">Whatever Page</Link>
      </li>
      <li>
        <Link to="/counter/">Counter</Link>
      </li>
    </ul>
  </div>
);
