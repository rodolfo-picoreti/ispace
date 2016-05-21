import { Meteor } from 'meteor/meteor'
import Services from '../db/services.js'

const amqp = Npm.require('amqplib/callback_api')
const broker = 'amqp://ispace:ispace@localhost/is'
const exchange = 'services'
const service = 'info'


Meteor.startup(() => {




  /*
  amqp.connect('amqp://ispace:ispace@localhost/is', Meteor.bindEnvironment((err, conn) => {
    conn.createChannel(Meteor.bindEnvironment((err, ch) => {

      ch.assertExchange(exchange, 'topic', { durable: true });
      ch.assertQueue('', { exclusive: true }, Meteor.bindEnvironment((err, q) => {
        if (err) console.log(err)

        ch.bindQueue(q.queue, exchange, q.queue);
        
        ch.publish(
          exchange, 
          service, 
          new Buffer('{}'), 
          { 
            correlationId: '5', 
            replyTo: q.queue,
            contentType: 'application/json' 
          }
        );
        
        console.log(' [x] Published!')
        
        ch.consume(q.queue, Meteor.bindEnvironment((msg) => {
          const info = JSON.parse(msg.content.toString())
          const selector = { _id: info._id }
          const options = { upsert: true }
          Services.update(selector, info, options)
        }), { noAck: true });

      }));
    }));
  }));
  */
})
