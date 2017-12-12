using System;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace WooCommerceNET.WooCommerce.Legacy
{
    [CollectionDataContract]
    public class WebhookList : List<Webhook>
    {
        [DataMember]
        public List<Webhook> webhooks { get; set; }
    }

    [DataContract]
    public class Webhook
    {
        /// <summary>
        /// Webhook ID (post ID) 
        /// read-only
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public int? id { get; set; }


        /// <summary>
        /// A friendly name for the webhook, defaults to “Webhook created on <date>”
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string name { get; set; }

      
        [DataMember(EmitDefaultValue = false)]
        public string status { get; set; }

        
        [DataMember(EmitDefaultValue = false)]
        public string topic { get; set; }

        /// <summary>
        /// Webhook resource, e.g. coupon
        /// read-only
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string resource { get; set; }

        /// <summary>
        /// Webhook event, e.g. updated
        /// read-only
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string @event { get; set; }


        /// <summary>
        /// WooCommerce action names associated with the webhook
        /// read-only
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public List<string> hooks { get; set; }

        /// <summary>
        /// The URL where the webhook payload is delivered       
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string delivery_url { get; set; }
 
        [DataMember(EmitDefaultValue = false)]
        public string secret { get; set; }

 
        [DataMember(EmitDefaultValue = false)]
        public string created_at { get; set; }

        /// <summary>
        /// UTC DateTime when the webhook was last updated
        /// read-only
        /// </summary>
        [DataMember(EmitDefaultValue = false)]

        public string updated_at { get; set; }


    }


    [CollectionDataContract]
    public class WebhookDeliveryList : List<WebhookDelivery>
    {
        [DataMember]
        public List<Webhook> webhookdeliveries { get; set; }
    }

    [DataContract]
    public class WebhookDelivery
    {
         
        [DataMember(EmitDefaultValue = false)]
        public int? id { get; set; }

        /// <summary>
        /// The delivery duration, in seconds        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string duration { get; set; }

        /// <summary>
        /// A friendly summary of the response including the HTTP response code, message, and body        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string summary { get; set; }

        /// <summary>
        /// The URL where the webhook was delivered
        /// read-only
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string request_url { get; set; }

        /// <summary>
        /// Array of request headers (see Request Headers Attributes)
        /// read-only
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public RequestHeaders request_headers { get; set; }

        /// <summary>
        /// The request body, this matches the API response for the given resource        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string request_body { get; set; }

        /// <summary>
        /// The HTTP response code from the receiving server        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string response_code { get; set; }

        /// <summary>
        /// The HTTP response message from the receiving server        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string response_message { get; set; }

        /// <summary>
        /// Array of the response headers from the receiving server        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public ResponseHeaders response_headers { get; set; }

        /// <summary>
        /// The response body from the receiving server        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string response_body { get; set; }

        /// <summary>
        /// A DateTime of when the delivery was logged        
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public DateTime? created_at { get; set; }

    }


    [DataContract]
    public class RequestHeaders
    {
        
        [DataMember(Name = "User-Agent", EmitDefaultValue = false)]
        public string UserAgent { get; set; }

        /// <summary>
        /// The request content-type, defaults to “application/json”
        /// </summary>
        [DataMember(Name = "Content-Type", EmitDefaultValue = false)]
        public string ContentType { get; set; }

        /// <summary>
        /// The webhook topic
        /// </summary>
        [DataMember(Name = "X-WC-Webhook-Topic", EmitDefaultValue = false)]
        public string XWCWebhookTopic { get; set; }

        /// <summary>
        /// The webhook resource
        /// </summary>
        [DataMember(Name = "X-WC-Webhook-Resource", EmitDefaultValue = false)]
        public string XWCWebhookResource { get; set; }

        /// <summary>
        /// The webhook event
        /// </summary>
        [DataMember(Name = "X-WC-Webhook-Event", EmitDefaultValue = false)]
        public string XWCWebhookEvent { get; set; }

        /// <summary>
        /// A base64 encoded HMAC-SHA256 hash of the payload
        /// </summary>
        [DataMember(Name = "X-WC-Webhook-Signature", EmitDefaultValue = false)]
        public string XWCWebhookSignature { get; set; }

        /// <summary>
        /// The webhook’s ID
        /// </summary>
        [DataMember(Name = "X-WC-Webhook-ID", EmitDefaultValue = false)]
        public int XWCWebhookID { get; set; }

        /// <summary>
        /// The delivery ID
        /// </summary>
        [DataMember(Name = "X-WC-Webhook-Delivery-ID", EmitDefaultValue = false)]
        public int XWCWebhookDeliveryID { get; set; }

    }

    [DataContract]
    public class ResponseHeaders
    {
        /// <summary>
        /// cache response directives values: private | public | no-cache
        /// </summary>
        [DataMember(Name = "cache-control", EmitDefaultValue = false)]
        public string cachecontrol { get; set; }

        /// <summary>
        ///  The response content-type, defaults to “application/json”
        /// </summary>
        [DataMember(Name = "content-type", EmitDefaultValue = false)]
        public string contenttype { get; set; }

        /// <summary>
        /// Webserver where webhook delivery page is hosted.
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string server { get; set; }

        /// <summary>
        /// Host engine where webhook delivery page is hosted, e.g. PHP / ASP.Net
        /// </summary>
        [DataMember(Name = "x-powered-by", EmitDefaultValue = false)]
        public string xpoweredby { get; set; }

        /// <summary>
        /// Date of webhook delivery posted
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string date { get; set; }

        /// <summary>
        /// Connection state
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string connection { get; set; }

        /// <summary>
        /// The request data length
        /// </summary>
        [DataMember(Name = "content-length", EmitDefaultValue = false)]
        public string contentlength { get; set; }
    }



}
