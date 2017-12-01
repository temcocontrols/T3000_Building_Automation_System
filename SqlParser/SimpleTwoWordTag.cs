using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region SimpleTwoWordTag

	/// <summary>
	/// The base class for such tags as Order By, Group By etc.
	/// </summary>
	internal abstract class SimpleTwoWordTag : SimpleOneWordTag
	{
		#region Methods

		#region Common

		/// <summary>
		/// Checks whether there is the tag at the specified position 
		/// in the specified sql.
		/// </summary>
		/// <returns>
		/// The position after the tag or -1 there is no tag at the position.
		/// </returns>
		protected override int MatchStart(string sql, int position)
		{
			return MatchStart(FirstWord, SecondWord, sql, position);
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

			output.Append(FirstWord);
			output.Append(ParserBase.cWhiteSpace);
			output.Append(SecondWord);
		}

		#endregion

		#region Static

		/// <summary>
		/// Checks whether there is the tag at the specified position 
		/// in the specified sql.
		/// </summary>
		/// <name>The value of the Name property.</name>
		/// <returns>
		/// The position after the tag or -1 there is no tag at the position.
		/// </returns>
		internal static int MatchStart(string firstWord, string secondWord, string sql, int position)
		{
			#region Check the arguments

			ParserBase.CheckTextAndPositionArguments(sql, position);

			#endregion

			if (string.Compare(sql, position, firstWord, 0, firstWord.Length, true) != 0)
				return -1;

			position += firstWord.Length;

			ParserBase.SkipWhiteSpace(sql, ref position);
			if (position == sql.Length)
				return -1;

			if (string.Compare(sql, position, secondWord, 0, secondWord.Length, true) != 0)
				return -1;

			return position + secondWord.Length;
		}

		#endregion

		#endregion

		#region Properties

		/// <summary>
		/// Gets the first word of the tag.
		/// </summary>
		protected abstract string FirstWord { get; }

		/// <summary>
		/// Gets the second word of the tag.
		/// </summary>
		protected abstract string SecondWord { get; }

		#endregion
	}

	#endregion

	#region MatchSimpleTwoWordTagAttribute

	internal abstract class MatchSimpleTwoWordTagAttribute : MatchSimpleOneWordTagAttribute
	{
		#region Methods

		public override bool Match(string sql, int position)
		{
			return SimpleTwoWordTag.MatchStart(FirstWord, SecondWord, sql, position) >= 0;
		}

		#endregion

		#region Properties

		/// <summary>
		/// Gets the first word of the tag.
		/// </summary>
		protected abstract string FirstWord { get; }

		/// <summary>
		/// Gets the second word of the tag.
		/// </summary>
		protected abstract string SecondWord { get; }

		#endregion
	}

	#endregion
}
