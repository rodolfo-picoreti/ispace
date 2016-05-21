import Express from 'express'
import GraphHTTP from 'express-graphql'
import Schema from '../db/schema/graphql/service'

const port = 4000
const app = Express()

// GraphQL
app.use('/graphql', GraphHTTP({
  schema: Schema,
  pretty: true,
  graphiql: true
}))

app.listen(port, ()=> {
  console.log(`Graphiql listening on port ${port}`)
})