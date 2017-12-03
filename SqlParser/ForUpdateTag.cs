using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region ForUpdateTag

	[TagType(ForUpdateTag.cTagName)]
	[MatchForUpdateTag]
	internal class ForUpdateTag : SimpleTwoWordTag
	{
		#region Consts

		/// <summary>
		/// The name of the tag (its identifier).
		/// </summary>
		public const string cTagName = "FOR_UPDATE";

		/// <summary>
		/// The first part of tag.
		/// </summary>
		public const string cTagFirstPart = "FOR";

		/// <summary>
		/// The second part of tag.
		/// </summary>
		public const string cTagSecondPart = "UPDATE";

		#endregion

		#region Properties

		/// <summary>
		/// Gets the name of the tag.
		/// </summary>
		protected override string Name 
		{
			get
			{
				return cTagName;
			}
		}

		/// <summary>
		/// Gets the first word of the tag.
		/// </summary>
		protected override string FirstWord
		{
			get
			{
				return cTagFirstPart;
			}
		}

		/// <summary>
		/// Gets the second word of the tag.
		/// </summary>
		protected override string SecondWord
		{
			get
			{
				return cTagSecondPart;
			}
		}

		#endregion
	}

	#endregion

	#region MatchForUpdateTagAttribute

	internal class MatchForUpdateTagAttribute : MatchSimpleTwoWordTagAttribute
	{
		#region Properties

		/// <summary>
		/// Gets the name of the tag (its identifier and sql text)
		/// </summary>
		protected override string Name
		{
			get
			{
				return ForUpdateTag.cTagName;
			}
		}

		/// <summary>
		/// Gets the first word of the tag.
		/// </summary>
		protected override string FirstWord 
		{
			get
			{
				return ForUpdateTag.cTagFirstPart;
			}
		}

		/// <summary>
		/// Gets the second word of the tag.
		/// </summary>
		protected override string SecondWord
		{
			get
			{
				return ForUpdateTag.cTagSecondPart;
			}
		}

		#endregion
	}

	#endregion
}
