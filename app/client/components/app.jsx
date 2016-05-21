import { Meteor } from 'meteor/meteor';

import React, { Component, PropTypes } from 'react';
import ReactDOM from 'react-dom';
import { createContainer } from 'meteor/react-meteor-data';

import Inspector from 'react-json-inspector'
 
import Services from '../../db/services.js';

class App extends Component {

  renderServicesInfo() {
    let info = this.props.services
    return (<Inspector data={info} />)
  }

  render() {
    return (
      <div> 
        { this.renderServicesInfo() }
      </div>
    )
  }
}

App.propTypes = {
  services: PropTypes.array.isRequired,
};

export default createContainer(() => {
  Meteor.subscribe('services');
  
  return {
    services: Services.find({}).fetch()
  }
}, App);