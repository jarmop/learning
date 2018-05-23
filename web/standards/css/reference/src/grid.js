import React, { Component } from 'react';

import './grid.css'

class Grid extends Component {
  render() {
    let {styles} = this.props;

    let cells = [];
    for (let i = 0; i < 9; i++) {
      cells.push(
          <div key={i} className="cell" style={styles.cell}>{i}</div>
      );
    }

    return (
        <div style={styles.grid}>
          {cells}
        </div>
    );
  }
}

export default Grid;