import React, { Component } from 'react';

import './grid.css'

class Grid extends Component {
  render() {
    let {styles} = this.props;

    let cells = [];
    for (let i = 0; i < 5; i++) {
      cells.push(
          <div key={i} className="cell" style={styles.cell}></div>
      );
    }

    return (
        <div className="grid" style={styles.grid}>
          {cells}
        </div>
    );
  }
}

export default Grid;