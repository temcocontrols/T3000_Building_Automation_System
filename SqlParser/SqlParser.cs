using System;
using System.Text;
using System.Xml;

namespace Parser
{
	#region SqlParser

	public class SqlParser: ParserBase
	{
		#region Base class implementation

		#region Fields

		/// <summary>
		/// Stores the types of all the possible tags.
		/// </summary>
		Type[] fTags;

		#endregion

		#region Properties

		/// <summary>
		/// Indicates whether the white space is a non-valueable character.
		/// </summary>
		protected override bool IsSkipWhiteSpace
		{
			get
			{
				return true;
			}
		}

		/// <summary>
		/// Returns the list of all the available tags.
		/// </summary>
		protected override Type[] Tags
		{
			get
			{
				if (fTags == null)
				{
					fTags = new Type[] { 
						typeof(SelectTag),
						typeof(FromTag),
						typeof(WhereTag),
						typeof(OrderByTag),
						typeof(BracesTag),
						typeof(StringLiteralTag),
						typeof(ForUpdateTag),
						typeof(StartWith),
						typeof(GroupByTag),
						typeof(QuotedIdentifierTag)
					};
				}

				return fTags;
			}
		}

		#endregion

		#endregion

		#region Common

		#region Methods

		/// <summary>
		/// Returns the xml node which corresponds to the From tag.
		/// If this node does not exist, this method generates an
		/// exception.
		/// </summary>
		private XmlNode GetFromTagXmlNode()
		{
			XmlNode myFromNode = ParsedDocument.SelectSingleNode(string.Format(@"{0}/{1}[@{2}='{3}']", cRootXmlNodeName, cTagXmlNodeName, cTagTypeXmlAttributeName, FromTag.cTagName));
			if (myFromNode == null)
				throw new Exception(ToText());

			return myFromNode;
		}

		/// <summary>
		/// Returns the xml node which corresponds to the For Update tag.	
		/// </summary>
		private XmlNode GetForUpdateTagXmlNode()
		{
			XmlNode myForUpdateNode = ParsedDocument.SelectSingleNode(string.Format(@"{0}/{1}[@{2}='{3}']", cRootXmlNodeName, cTagXmlNodeName, cTagTypeXmlAttributeName, ForUpdateTag.cTagName));

			return myForUpdateNode;
		}

		/// <summary>
		/// Checks whether there is a tag in the text at the specified position, and returns its tag.
		/// </summary>
		internal new Type IsTag(string text, int position)
		{
			return base.IsTag(text, position);
		}

		#endregion

		#endregion

		#region Where Clause

		#region Methods

		/// <summary>
		/// Returns the xml node which corresponds to the Where tag.
		/// If this node does not exist, creates a new one (if needed).
		/// </summary>
		private XmlNode GetWhereTagXmlNode(bool createNew)
		{
			XmlNode myWhereNode = ParsedDocument.SelectSingleNode(string.Format(@"{0}/{1}[@{2}='{3}']", cRootXmlNodeName, cTagXmlNodeName, cTagTypeXmlAttributeName, WhereTag.cTagName));
			if (myWhereNode == null && createNew)
			{
				WhereTag myWhereTag = new WhereTag();
				myWhereTag.InitializeFromData(this, null, false);
				myWhereNode = CreateTagXmlNode(myWhereTag);

				XmlNode myFromNode = GetFromTagXmlNode();
				myFromNode.ParentNode.InsertAfter(myWhereNode, myFromNode);
			}

			return myWhereNode;
		}

		#endregion

		#region Properties

		/// <summary>
		/// Gets or sets the Where clause of the parsed sql.
		/// </summary>
		public string WhereClause
		{
			get
			{
				#region Get the Where xml node

				XmlNode myWhereTagXmlNode = GetWhereTagXmlNode(false);
				if (myWhereTagXmlNode == null)
					return string.Empty;

				#endregion

				StringBuilder myStringBuilder = new StringBuilder();
				XmlNodesToText(myStringBuilder, myWhereTagXmlNode.ChildNodes);
				return myStringBuilder.ToString();
			}
			set
			{
				if (string.IsNullOrEmpty(value))
				{
					#region Remove the Where xml node

					XmlNode myWhereXmlNode = GetWhereTagXmlNode(false);
					if (myWhereXmlNode != null)
						myWhereXmlNode.ParentNode.RemoveChild(myWhereXmlNode);

					#endregion
				}
				else
				{
					#region Modify the Where xml node

					XmlNode myWhereXmlNode = GetWhereTagXmlNode(true);

					ClearXmlNode(myWhereXmlNode);

					TagBase myWhereTag = TagXmlNodeToTag(myWhereXmlNode);

					ParseBlock(myWhereXmlNode, myWhereTag, value, 0);

					#endregion
				}
			}
		}

		#endregion

		#endregion

		#region Order By Clause

		#region Methods

		/// <summary>
		/// Returns the xml node which corresponds to the Order By tag.
		/// If this node does not exist, creates a new one (if needed).
		/// </summary>
		private XmlNode GetOrderByTagXmlNode(bool createNew)
		{
			XmlNode myOrderByNode = ParsedDocument.SelectSingleNode(string.Format(@"{0}/{1}[@{2}='{3}']", cRootXmlNodeName, cTagXmlNodeName, cTagTypeXmlAttributeName, OrderByTag.cTagName));
			if (myOrderByNode == null && createNew)
			{
				OrderByTag myOrderByTag = new OrderByTag();
				myOrderByTag.InitializeFromData(this, null, false);
				myOrderByNode = CreateTagXmlNode(myOrderByTag);

				XmlNode myForUpdateNode = GetForUpdateTagXmlNode();
				if (myForUpdateNode != null)
				{
					myForUpdateNode.ParentNode.InsertBefore(myOrderByNode, myForUpdateNode);
					return myOrderByNode;
				}

				XmlNode myFromNode = GetFromTagXmlNode();
				myFromNode.ParentNode.AppendChild(myOrderByNode);
			}

			return myOrderByNode;
		}

		#endregion

		#region Properties

		/// <summary>
		/// Gets or sets the Order By clause of the parsed sql.
		/// </summary>
		public string OrderByClause
		{
			get
			{
				#region Get the Order By xml node

				XmlNode myOrderByTagXmlNode = GetOrderByTagXmlNode(false);
				if (myOrderByTagXmlNode == null)
					return string.Empty;

				#endregion

				StringBuilder myStringBuilder = new StringBuilder();
				XmlNodesToText(myStringBuilder, myOrderByTagXmlNode.ChildNodes);
				return myStringBuilder.ToString();
			}
			set
			{
				if (string.IsNullOrEmpty(value))
				{
					#region Remove the Order By xml node

					XmlNode myOrderByXmlNode = GetOrderByTagXmlNode(false);
					if (myOrderByXmlNode != null)
						myOrderByXmlNode.ParentNode.RemoveChild(myOrderByXmlNode);

					#endregion
				}
				else
				{
					#region Modify the Order By xml node

					XmlNode myOrderByXmlNode = GetOrderByTagXmlNode(true);

					ClearXmlNode(myOrderByXmlNode);

					TagBase myOrderByTag = TagXmlNodeToTag(myOrderByXmlNode);

					ParseBlock(myOrderByXmlNode, myOrderByTag, value, 0);

					#endregion
				}
			}
		}

		#endregion

		#endregion
	}

	#endregion
}
