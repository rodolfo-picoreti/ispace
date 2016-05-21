
const Output = new SimpleSchema({
  name: {
    type: String,
    label: "Name",
    max: 50,
    optional: true
  },  
  description: {
    type: String,
    label: "Description",
    max: 200
  },
  type: {
    type: String,
    label: "Type"
  }
})

const Input = new SimpleSchema({
  name: {
    type: String,
    label: "Name",
    max: 50,
    optional: true
  },  
  description: {
    type: String,
    label: "Description",
    max: 200
  },
  type: {
    type: String,
    label: "Type"
  },
  required: {
    type: Boolean,
    label: "Required",
    optional: true,
    defaultValue: true
  }
})

const Service = new SimpleSchema({
  name: {
    type: String,
    label: "Name",
    max: 50
  },
  description: {
    type: String,
    label: "Description",
    max: 200
  },
  outputs: {
    type: [Output],
    label: "Outputs"
  },
  inputs: {
    type: [Input],
    label: "Inputs"
  }
})

export default Service