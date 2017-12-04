using System;
using System.Text;
using System.Xml;

namespace Parser
{
	#region ParserBase

	/// <summary>
	/// An abstract class which is used as the base class for parsers.
	/// It contains basic methods which allow you to create a logical tree
	/// from a document.
	/// </summary>
	public abstract class ParserBase
	{
		#region Consts

		/// <summary>
		/// The name of the Value xml attribute.
		/// </summary>
		protected const string cValueXmlAttributeName = "Value";

		/// <summary>
		/// The name of the Tag Type xml attribute.
		/// </summary>
		protected const string cTagTypeXmlAttributeName = "Type";

		/// <summary>
		/// The name of the root node in the xml representation of a parsed text.
		/// </summary>
		protected const string cRootXmlNodeName = "ParsedDocument";
		
		/// <summary>
		/// The name of the Tag xml node.
		/// </summary>
		protected const string cTagXmlNodeName = "Tag";

		/// <summary>
		/// The name of the Text xml node.
		/// </summary>
		protected const string cTextXmlNodeName = "Text";

		/// <summary>
		/// The white space text (is used when adding white 
		/// spaces between text elements).
		/// </summary>
		public const string cWhiteSpace = " ";

		/// <summary>
		/// The new line string.
		/// </summary>
		public const string cNewLine = "\r\n";

		#endregion

		#region Fields

		/// <summary>
		/// The tree representation of a parsed document as an xml document.
		/// </summary>
		private XmlDocument fParsedDocument;

		#endregion

		#region Methods

		/// <summary>
		/// Parses the specified text.
		/// </summary>
		public void Parse(string text)
		{
			#region Check the arguments

			if (text == null)
				throw new ArgumentNullException("text");

			#endregion

			fParsedDocument = new XmlDocument();
			XmlNode myRootNode = fParsedDocument.CreateElement(cRootXmlNodeName);
			fParsedDocument.AppendChild(myRootNode);

			try
			{
				ParseBlock(myRootNode, null, text, 0);
			}
			catch
			{
				fParsedDocument = null;
				throw;
			}

			// m_ParsedDocument.Save(@"D:\Temp\ParserTest.xml");
		}

		/// <summary>
		/// Parses the specified block of a text.
		/// </summary>
		/// <returns>
		/// Returns the end position of the parsed block.
		/// </returns>
		protected int ParseBlock(XmlNode parentNode, TagBase parentTag, string text, int position)
		{
			#region Check the arguments

			if (parentNode == null)
				throw new ArgumentNullException("parentNode");

			CheckTextAndPositionArguments(text, position);

			#endregion

			while (position < text.Length)
			{
				if (IsSkipWhiteSpace)
					SkipWhiteSpace(text, ref position);

				if (position == text.Length)
					break;

				#region Read the parent tag ending

				if (parentTag != null)
				{
					int myParentTagEndingEndPosition = parentTag.MatchEnd(text, position);
					if (myParentTagEndingEndPosition >= 0)
					{
						position = myParentTagEndingEndPosition;
						return position;
					}
				}

				#endregion

				Type myTagType = IsTag(text, position);
				if (myTagType != null)
				{
					#region Read a tag

					#region Create the tag class instance

					TagBase myTag = Activator.CreateInstance(myTagType) as TagBase;
					position = myTag.InitializeFromText(this, text, position, parentTag);

					#endregion

					#region Create an xml node for the tag

					XmlNode myTagXmlNode = CreateTagXmlNode(myTag);
					parentNode.AppendChild(myTagXmlNode);

					#endregion

					if (myTag.HasContents)
						position = ParseBlock(myTagXmlNode, myTag, text, position);

					#endregion
				}
				else
				{
					#region Read text

					string myText = ReadWordOrSeparator(text, ref position, !IsSkipWhiteSpace);
					parentNode.AppendChild(CreateTextXmlNode(myText));

					#endregion
				}
			}

			if (parentTag != null && !parentTag.CanTerminateByStringEnd)
				throw new Exception("Invalid format");

			return position;
		}

		/// <summary>
		/// Checks whether there is a tag in the text at the specified position, and returns its type.
		/// </summary>
		protected Type IsTag(string text, int position)
		{
			foreach (Type myTagType in Tags)
			{
				MatchTagAttributeBase myMatchTagAttribute = TagBase.GetTagMatchAttribute(myTagType);
				if (myMatchTagAttribute.Match(text, position))
					return myTagType;
			}

			return null;
		}

		/// <summary>
		/// Creates an xml node for the specified tag.
		/// </summary>
		protected XmlNode CreateTagXmlNode(TagBase tag)
		{
			#region Check the arguments

			if (tag == null)
				throw new ArgumentNullException();

			#endregion

			CheckXmlDocInitialized();

			XmlElement myTagNode = fParsedDocument.CreateElement(cTagXmlNodeName);

			XmlAttribute myTypeAttribute = fParsedDocument.CreateAttribute(cTagTypeXmlAttributeName);
			myTypeAttribute.Value = tag.Type;
			myTagNode.Attributes.Append(myTypeAttribute);

			if (tag.Value != null)
			{
				XmlAttribute myValueAttribute = fParsedDocument.CreateAttribute(cValueXmlAttributeName);
				myValueAttribute.Value = tag.Value;
				myTagNode.Attributes.Append(myValueAttribute);
			}

			return myTagNode;
		}

		/// <summary>
		/// Creates an xml node for the specified text.
		/// </summary>
		protected XmlNode CreateTextXmlNode(string text)
		{
			#region Check the arguments

			if (text == null)
				throw new ArgumentNullException("text");

			#endregion

			CheckXmlDocInitialized();

			XmlElement myTextNode = fParsedDocument.CreateElement(cTextXmlNodeName);

			XmlAttribute myValueAttribute = fParsedDocument.CreateAttribute(cValueXmlAttributeName);
			myValueAttribute.Value = text;
			myTextNode.Attributes.Append(myValueAttribute);

			return myTextNode;
		}

		/// <summary>
		/// Skips the white space symbols located at the specified position.
		/// </summary>
		public static void SkipWhiteSpace(string text, ref int position)
		{
			#region Check the parameters

			CheckTextAndPositionArguments(text, position);

			#endregion

			while (position < text.Length)
			{
				if (!char.IsWhiteSpace(text, position))
					break;
				position++;
			}
		}

		/// <summary>
		/// Reads a single word or separator at the specified position.
		/// </summary>
		public static string ReadWordOrSeparator(string text, ref int position, bool treatWhiteSpaceAsSeparator)
		{
			#region Check the parameters

			CheckTextAndPositionArguments(text, position);

			#endregion

			int myStartPosition = position;

			while (position < text.Length)
			{
				#region Check is white space

				if (char.IsWhiteSpace(text, position))
				{
					if (position == myStartPosition && treatWhiteSpaceAsSeparator)
					{
						if (position + cNewLine.Length <= text.Length && text.Substring(position, cNewLine.Length) == cNewLine)
							position += cNewLine.Length;
						else
							position += 1;
					}
					break;
				}

				#endregion

				#region Check is separator

				if (!char.IsLetterOrDigit(text, position) && text[position] != '_')
				{
					if (position == myStartPosition)
						position++;
					break;
				}

				#endregion

				position++;
			}

			if (position == myStartPosition)
				return string.Empty;

			return text.Substring(myStartPosition, position - myStartPosition);
		}

		/// <summary>
		/// Checks the text and position parameters.
		/// </summary>
		public static void CheckTextAndPositionArguments(string text, int position)
		{
			#region Check the parameters

			if (text == null)
				throw new ArgumentNullException("text");

			if (position < 0 || position >= text.Length)
				throw new ArgumentOutOfRangeException("position");

			#endregion
		}

		/// <summary>
		/// Checks whether the m_ParsedDocument is initialized.
		/// </summary>
		protected void CheckXmlDocInitialized()
		{
			if (fParsedDocument == null)
				throw new Exception("Xml document is not initialized");
		}

		/// <summary>
		/// Returns the text string processed by the parser.
		/// </summary>
		public string ToText()
		{
			CheckXmlDocInitialized();

			StringBuilder myStringBuilder = new StringBuilder();

			XmlNodesToText(myStringBuilder, fParsedDocument.ChildNodes[0].ChildNodes);

			return myStringBuilder.ToString();
		}

		/// <summary>
		/// Converts the specified xml node collection to the text.
		/// </summary>
		protected void XmlNodesToText(StringBuilder output, XmlNodeList nodes)
		{
			#region Check arguments

			if (nodes == null)
				throw new ArgumentNullException("nodes");

			if (output == null)
				throw new ArgumentNullException("output");

			#endregion

			foreach (XmlNode myNode in nodes)
				XmlNodeToText(output, myNode);
		}

		/// <summary>
		/// Converts the specified xml node to text.
		/// </summary>
		protected void XmlNodeToText(StringBuilder output, XmlNode node)
		{
			#region Check arguments

			if (node == null)
				throw new ArgumentNullException("node");

			if (output == null)
				throw new ArgumentNullException("output");

			#endregion

			if (IsTagXmlNode(node))
			{
				TagXmlNodeToText(output, node);
			}
			else if (IsTextXmlNode(node))
			{
				output.Append(GetTextNodeText(node));

				if (IsSkipWhiteSpace)
					output.Append(cWhiteSpace);
			}
			else
			{
				throw new Exception("Unrecognized xml node.");
			}

			//m_ParsedDocument.Save(@"D:\Temp\testparser.xml");
		}

		/// <summary>
		/// Determines whether the specified node is a tag node.
		/// </summary>
		protected bool IsTagXmlNode(XmlNode node)
		{
			#region Check arguments

			if (node == null)
				throw new ArgumentNullException("node");

			#endregion

			return node.Name == cTagXmlNodeName;
		}

		/// <summary>
		/// Determines whether the specified node is a text node.
		/// </summary>
		protected bool IsTextXmlNode(XmlNode node)
		{
			#region Check arguments

			if (node == null)
				throw new ArgumentNullException("node");

			#endregion

			return node.Name == cTextXmlNodeName;
		}

		/// <summary>
		/// Returns the text stored in the text node.
		/// </summary>
		protected string GetTextNodeText(XmlNode node)
		{
			string myValue = GetXmlNodeValue(node);
			if (myValue == null)
				throw new Exception("Cannot get text node's value.");

			return myValue;
		}

		/// <summary>
		/// Retrieves the value of the specified tag xml node.
		/// </summary>
		protected string GetXmlNodeValue(XmlNode node)
		{
			#region Check arguments

			if (node == null)
				throw new ArgumentNullException("node");

			#endregion

			XmlAttribute myValueXmlAttribute = node.Attributes[cValueXmlAttributeName];
			if (myValueXmlAttribute == null)
				return null;

			return myValueXmlAttribute.Value;
		}

		/// <summary>
		/// Converts the specified tag xml node to text.
		/// </summary>
		private void TagXmlNodeToText(StringBuilder output, XmlNode node)
		{
			#region Check arguments

			if (node == null)
				throw new ArgumentNullException("node");

			if (output == null)
				throw new ArgumentNullException("output");

			#endregion

			TagBase myTag = TagXmlNodeToTag(node);

			myTag.WriteStart(output);

			if (IsSkipWhiteSpace)
				output.Append(cWhiteSpace);

			if (node.ChildNodes.Count > 0)
				XmlNodesToText(output, node.ChildNodes);

			myTag.WriteEnd(output);

			if (IsSkipWhiteSpace)
				output.Append(cWhiteSpace);
		}

		/// <summary>
		/// Converts the specified tag xml node into a tag.
		/// </summary>
		protected TagBase TagXmlNodeToTag(XmlNode node)
		{
			#region Check arguments

			if (node == null)
				throw new ArgumentNullException("node");

			#endregion

			#region Get tag parameters

			XmlAttribute myTagTypeXmlAttribute = node.Attributes[cTagTypeXmlAttributeName];
			if (myTagTypeXmlAttribute == null)
				throw new Exception("Cannot find the tag type attribute");
			string myTagType = myTagTypeXmlAttribute.Value;

			XmlAttribute myValueXmlAttribute = node.Attributes[cValueXmlAttributeName];
			string myTagValue;
			if (myValueXmlAttribute == null)
				myTagValue = null;
			else
				myTagValue = myValueXmlAttribute.Value;

			#endregion

			return GetTagFromType(myTagType, myTagValue, node.ChildNodes.Count > 0);
		}

		/// <summary>
		/// Retrurns the tag instance from its type and value.
		/// </summary>
		protected TagBase GetTagFromType(string type, string value, bool hasContents)
		{
			foreach (Type myTagType in Tags)
			{
				string myType = TagBase.GetTagType(myTagType);
				if (string.Compare(myType, type, true) == 0)
				{
					TagBase myTag = Activator.CreateInstance(myTagType) as TagBase;
					myTag.InitializeFromData(this, value, hasContents);
					return myTag;
				}
			}

			throw new Exception(string.Format("Tag cannot be found: {0}", type));
		}

		/// <summary>
		/// Removes all the sub nodes from the specified node. 
		/// </summary>
		protected static void ClearXmlNode(XmlNode node)
		{
			#region Check arguments

			if (node == null)
				throw new ArgumentNullException("node");

			#endregion

			for (int myChildIndex = node.ChildNodes.Count - 1; myChildIndex >= 0; myChildIndex--)
				node.RemoveChild(node.ChildNodes[myChildIndex]);
		}

		/// <summary>
		/// Returns how many continuous specified strings are before of the specified
		/// position in the specified text
		/// </summary>
		public static int CountStringsBefore(string text, int position, string @string)
		{
			CheckTextAndPositionArguments(text, position);

			#region Count the Strings before

			int myStringsBeforeCount = 0;
			int myPrevPosition = position - @string.Length;
			while (myPrevPosition >= 0)
			{
				// If it is not the specified string prefix
				if (string.Compare(text, myPrevPosition, @string, 0, @string.Length, true) != 0)
					break;

				myStringsBeforeCount++;
				myPrevPosition -= @string.Length;
			}

			#endregion

			return myStringsBeforeCount;
		}

		#endregion

		#region Properties

		/// <summary>
		/// Indicates whether the white space is considered as a non-valuable
		/// character.
		/// </summary>
		protected abstract bool IsSkipWhiteSpace { get; }

		/// <summary>
		/// The list of all available tags.
		/// </summary>
		protected abstract Type[] Tags { get; }

		/// <summary>
		/// Gets the parsed document in xml format.
		/// </summary>
		protected XmlDocument ParsedDocument
		{
			get
			{
				CheckXmlDocInitialized();

				return fParsedDocument;
			}
		}

		#endregion
	}

	#endregion
}
