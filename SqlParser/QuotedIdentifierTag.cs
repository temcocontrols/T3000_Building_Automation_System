using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region QuotedIdentifierTag

	[TagType(QuotedIdentifierTag.cTagName)]
	[MatchQuotedIdentifierTag]
	internal class QuotedIdentifierTag : TagBase
	{
		#region Consts

		/// <summary>
		/// The name of the tag (its identifier).
		/// </summary>
		public const string cTagName = "QUOTED_IDENTIFIER";

		/// <summary>
		/// The tag delimiter.
		/// </summary>
		public const string cTagDelimiter = "\"";

		#endregion

		#region Methods

		#region Common

		/// <summary>
		/// Reads the tag at the specified position in the specified word and separator array.
		/// </summary>
		/// <returns>
		/// The position after the tag (at which to continue reading).
		/// </returns>
		protected override int InitializeCoreFromText(ParserBase parser, string sql, int position, TagBase parentTag)
		{
			#region Check the arguments

			ParserBase.CheckTextAndPositionArguments(sql, position);

			#endregion

			int myAfterTagStartPos = MatchStartStatic(sql, position);

			if (myAfterTagStartPos < 0)
				throw new Exception("Cannot read the QuotedIdentifier tag.");

			#region Read the identifier's value

			int myTagEndPos = sql.IndexOf(cTagDelimiter, myAfterTagStartPos, StringComparison.InvariantCultureIgnoreCase);
			if (myTagEndPos < 0)
				throw new Exception("Cannot read the QuotedIdentifier tag.");

			if (myAfterTagStartPos == myTagEndPos)
				Value = string.Empty;
			else
				Value = sql.Substring(myAfterTagStartPos, myTagEndPos - myAfterTagStartPos);

			#endregion

			Parser = parser;

			HasContents = false;

			return myTagEndPos + cTagDelimiter.Length;
		}

		/// <summary>
		/// Writes the start of the tag.
		/// </summary>
		public override void WriteStart(StringBuilder output)
		{
			CheckInitialized();

			#region Check the parameters

			if (output == null)
				throw new ArgumentNullException();

			#endregion

			output.Append(cTagDelimiter);
			output.Append(Value);
			output.Append(cTagDelimiter);
		}

		#endregion

		#region Static

		/// <summary>
		/// Checks whether there is the tag start at the specified position 
		/// in the specified sql.
		/// </summary>
		/// <returns>
		/// The position after the tag or -1 there is no tag start at the position.
		/// </returns>
		public static int MatchStartStatic(string sql, int position)
		{
			#region Check the arguments

			ParserBase.CheckTextAndPositionArguments(sql, position);

			#endregion

			if (string.Compare(sql, position, cTagDelimiter, 0, cTagDelimiter.Length, true) != 0)
				return -1;

			return position + cTagDelimiter.Length;
		}

		#endregion

		#endregion		
	}

	#endregion

	#region MatchQuotedIdentifierTagAttribute

	internal class MatchQuotedIdentifierTagAttribute : MatchTagAttributeBase
	{
		#region Methods

		public override bool Match(string sql, int position)
		{
			return QuotedIdentifierTag.MatchStartStatic(sql, position) >= 0;
		}

		#endregion
	}

	#endregion
}
