using System;
using System.Xml;
using System.Text;

namespace Parser
{
	#region TagBase

	/// <summary>
	/// The base class for all the tags.
	/// </summary>
	public abstract class TagBase
	{
		#region Fields

		/// <summary>
		/// The value of the IsInitialized property.
		/// </summary>
		private bool fIsInitialized = false;

		/// <summary>
		/// The value of the HasContents property.
		/// </summary>
		private bool fHasContents = false;

		/// <summary>
		/// The value of the Value property.
		/// </summary>
		private string fValue;

		/// <summary>
		/// The value of the Parser property.
		/// </summary>
		private ParserBase fParser;

		#endregion

		#region Methods

		#region Initialization

		/// <summary>
		/// Reads the tag at the specified position in the specified word and separator array.
		/// </summary>
		/// <param name="parentTag">
		/// The parent tag of this tag. This argument is used to determine 
		/// the end of the tag (it can be the end of the parent tag).
		/// </param>
		/// <returns>
		/// The position after the tag (at which to continue reading).
		/// </returns>
		public int InitializeFromText(ParserBase parser, string text, int position, TagBase parentTag)
		{
			#region Check the arguments

			if (parser == null)
				throw new ArgumentNullException("parser");

			#endregion

			int myResult = InitializeCoreFromText(parser, text, position, parentTag);

			IsInitialized = true;

			return myResult;
		}

		/// <summary>
		/// Reads the tag at the specified position in the specified word and separator array.
		/// </summary>
		/// <param name="parentTag">
		/// The parent tag of this tag. This argument is used to determine 
		/// the end of the tag (it can be the end of the parent tag).
		/// </param>
		/// <returns>
		/// The position after the tag (at which to continue reading).
		/// </returns>
		protected abstract int InitializeCoreFromText(ParserBase parser, string text, int position, TagBase parentTag);

		/// <summary>
		/// Reads the tag from the specified data.
		/// </summary>
		public void InitializeFromData(ParserBase parser, string value, bool hasContents)
		{
			#region Check the arguments

			if (parser == null)
				throw new ArgumentNullException("parser");

			#endregion

			InitializeFromDataCore(parser, value, hasContents);

			IsInitialized = true;
		}

		/// <summary>
		/// Reads the tag from the specified data.
		/// </summary>
		protected virtual void InitializeFromDataCore(ParserBase parser, string value, bool hasContents)
		{
			Parser = parser;

			Value = value;

			HasContents = hasContents;

			IsInitialized = true;
		}

		#endregion

		#region Attributes-Related (Static)

		/// <summary>
		/// Returns the type of the specified tag (its identifier). 
		/// This method is used by the Type property.
		/// </summary>
		public static string GetTagType(Type tag)
		{
			#region Check the arguments

			if (tag == null)
				throw new ArgumentNullException("tag");

			#endregion

			object[] myTagTypeAttributes = tag.GetCustomAttributes(typeof(TagTypeAttribute), true);

			if (myTagTypeAttributes == null || myTagTypeAttributes.Length == 0)
				throw new Exception("Cannot find a tag type attribute.");

			if (myTagTypeAttributes.Length > 1)
				throw new Exception("Ambiguous tag type.");

			return (myTagTypeAttributes[0] as TagTypeAttribute).Type;
		}

		/// <summary>
		/// Returns the match attribute of the specified class. 
		/// This attribute is used to identify a tag in a text.
		/// </summary>
		public static MatchTagAttributeBase GetTagMatchAttribute(Type tag)
		{
			#region Check the arguments

			if (tag == null)
				throw new ArgumentNullException("tag");

			#endregion

			object[] myMatchTagAttributes = tag.GetCustomAttributes(typeof(MatchTagAttributeBase), true);

			if (myMatchTagAttributes == null || myMatchTagAttributes.Length == 0)
				throw new Exception("Cannot find a match tag attribute.");

			if (myMatchTagAttributes.Length > 1)
				throw new Exception("Ambiguous match tag.");

			return myMatchTagAttributes[0] as MatchTagAttributeBase;
		}

		#endregion

		#region Common

		/// <summary>
		/// Returns a value indicating whether there is the tag ending 
		/// at the specified position.
		/// </summary>
		/// <returns>
		/// If this value is less than zero, then there is no ending; otherwise the 
		/// position after ending is returned.
		/// </returns>
		public virtual int MatchEnd(string text, int position)
		{
			CheckInitialized();

			throw new System.NotImplementedException();
		}

		/// <summary>
		/// Writes the start of the tag.
		/// </summary>
		public abstract void WriteStart(StringBuilder output);

		/// <summary>
		/// Writes the end of the tag.
		/// </summary>
		public virtual void WriteEnd(StringBuilder output)
		{
			CheckInitialized();
		}

		/// <summary>
		/// Checks whether the tag is initialized, and throws an exception if not. This method should be called before a tag porperty or method is accessed.
		/// </summary>
		protected void CheckInitialized()
		{
			if (!IsInitialized)
				throw new Exception("Tag is not initialized.");
		}

		#endregion

		#endregion

		#region Properties

		/// <summary>
		/// Returns the type of the tag (its identifier).
		/// </summary>
		public string Type
		{
			get
			{
				return GetTagType(GetType());
			}
		}

		/// <summary>
		/// The value of the tag (it can be the text of a string literal for example).
		/// </summary>
		public string Value
		{
			get
			{
				CheckInitialized();

				return fValue;
			}
			protected set
			{
				fValue = value;
			}
		}

		/// <summary>
		/// Gets a value indicating whether this tag has contents to be parsed.
		/// </summary>
		public virtual bool HasContents
		{
			get
			{
				CheckInitialized();

				return fHasContents;
			}
			protected set
			{
				fHasContents = value;
			}
		}

		/// <summary>
		/// Inidicates whether the string end can be treated as the end of the tag.
		/// </summary>
		public virtual bool CanTerminateByStringEnd
		{
			get
			{
				CheckInitialized();

				return false;
			}
		}

		/// <summary>
		/// Indicates whether the tag has been initialized.
		/// </summary>
		protected bool IsInitialized
		{
			get
			{
				return fIsInitialized;
			}
			private set
			{
				fIsInitialized = value;
			}
		}

		/// <summary>
		/// The parser which has created this instance.
		/// </summary>
		public ParserBase Parser
		{
			get
			{
				CheckInitialized();

				return fParser;
			}
			protected set
			{
				fParser = value;
			}
		}

		#endregion
	}

	#endregion

	#region MatchTagAttributeBase

	/// <summary>
	/// This attribute is used to determine whether there is certain tag 
	/// at a position in a text.
	/// </summary>
	public abstract class MatchTagAttributeBase : Attribute
	{
		public abstract bool Match(string text, int position);
	}

	#endregion

	#region TagTypeAttribute

	/// <summary>
	/// This attribute is used to determine the name (identifier) of a tag.
	/// </summary>
	public sealed class TagTypeAttribute : Attribute
	{
		#region Fields

		private string fType;
		
		#endregion

		#region Methods

		public TagTypeAttribute(string type)
		{
			Type = type;
		} 

		#endregion

		#region Properties

		public string Type
		{
			get
			{
				return fType;
			}
			private set
			{
				fType = value;
			}
		}
		
		#endregion
	}

	#endregion
}