import config from './floatingpointconfig';
import React from 'react';

export default class extends React.Component {
	constructor() {
		super();
	}

  updateConfig(evt) {
    console.log();
    config.mantissaBits = evt.target.value;
    this.forceUpdate();
  }

  render() {
    return (
      <div className="options">
        <input type={"range"} name={"mantissaBits"} min={1} max={23} onChange={this.updateConfig.bind(this)}/>
        <span className={"mantissaBits"}>Mantissa Bits: {config.mantissaBits}</span>
      </div>
    );
  }

}