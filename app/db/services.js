import { Meteor } from 'meteor/meteor'
import { Mongo } from 'meteor/mongo'

import schema from './schema/mongo/service'

const Services = new Mongo.Collection('services')
Services.attachSchema(schema)


if (Meteor.isServer) {
  Meteor.publish('services', () => {
    return Services.find({})
  })
}

Meteor.methods({
  'services.insert'(service) {
    Services.insert(service);
  }
})

export default Services

