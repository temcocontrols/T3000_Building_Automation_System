using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region GroupByTag

	[TagType(GroupByTag.cTagName)]
	[MatchGroupByTag]
	internal class GroupByTag : SimpleTwoWordTag
	{
		#region Consts

		/// <summary>
		/// The name of the tag (its identifier).
		/// </summary>
		public const string cTagName = "GROUP_BY";

		/// <summary>
		/// The first part of tag.
		/// </summary>
		public const string cTagFirstPart = "GROUP";

		/// <summary>
		/// The second part of tag.
		/// </summary>
		public const string cTagSecondPart = "BY";

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

	#region MatchGroupByTagAttribute

	internal class MatchGroupByTagAttribute : MatchSimpleTwoWordTagAttribute
	{
		#region Properties

		/// <summary>
		/// Gets the name of the tag (its identifier and sql text)
		/// </summary>
		protected override string Name
		{
			get
			{
				return GroupByTag.cTagName;
			}
		}

		/// <summary>
		/// Gets the first word of the tag.
		/// </summary>
		protected override string FirstWord 
		{
			get
			{
				return GroupByTag.cTagFirstPart;
			}
		}

		/// <summary>
		/// Gets the second word of the tag.
		/// </summary>
		protected override string SecondWord
		{
			get
			{
				return GroupByTag.cTagSecondPart;
			}
		}

		#endregion
	}

	#endregion
}
