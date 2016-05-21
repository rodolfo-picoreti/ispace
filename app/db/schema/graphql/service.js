import {
  GraphQLObjectType,
  GraphQLString,
  GraphQLList,
  GraphQLInt,
  GraphQLSchema,
  GraphQLNonNull,
  GraphQLBoolean
} from 'graphql'

import Services from '../../services.js';

const Input = new GraphQLObjectType({
  name: 'Input',
  description: 'Input Model',
  fields: () => {
    return {
      name: {
        type: GraphQLString,
        resolve (input) {
          return input.name
        }
      },
      description: {
        type: GraphQLString,
        resolve (input) {
          return input.description
        }
      },
      type: {
        type: GraphQLString,
        resolve (input) {
          return input.type
        }
      },
      required: {
        type: GraphQLBoolean,
        defaultValue: true,
        resolve (input) {
          return input.required
        }
      }
    }
  }
})

const Output = new GraphQLObjectType({
  name: 'Output',
  description: 'Output Model',
  fields: () => {
    return {
      name: {
        type: GraphQLString,
        resolve (output) {
          return output.name
        }
      },
      description: {
        type: GraphQLString,
        resolve (output) {
          return output.description
        }
      },
      type: {
        type: GraphQLString,
        resolve (output) {
          return output.type
        }
      }
    }
  }
})

const Service = new GraphQLObjectType({
  name: 'Service',
  description: 'Service Model',
  fields: () => {
    return {
      name: {
        type: GraphQLString,
        resolve (service) {
          return service.name
        }
      },
      description: {
        type: GraphQLString,
        resolve (service) {
          return service.description
        }
      },
      outputs: {
        type: new GraphQLList(Output),
        resolve (service) {
          return service.outputs
        }
      },
      inputs: {
        type: new GraphQLList(Input),
        resolve (service) {
          return service.inputs
        }
      }
    }
  }
})

const Query = new GraphQLObjectType({
  name: 'Query',
  description: 'Root Query',
  fields: () => {
    return {
      services: {
        type: new GraphQLList(Service),
        args: {
          name: { type: GraphQLString }
        },
        resolve(root, { name }) {
          return Services.find({}).fetch();
        }
      }
    }
  }
})

const Schema = new GraphQLSchema({
  query: Query,
  mutation: null
})

export default Schema;