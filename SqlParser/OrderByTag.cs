using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region OrderByTag

	[TagType(OrderByTag.cTagName)]
	[MatchOrderByTag]
	internal class OrderByTag : SimpleTwoWordTag
	{
		#region Consts

		/// <summary>
		/// The name of the tag (its identifier).
		/// </summary>
		public const string cTagName = "ORDER_BY";

		/// <summary>
		/// The first part of tag.
		/// </summary>
		public const string cTagFirstPart = "ORDER";

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

	#region MatchOrderByTagAttribute

	internal class MatchOrderByTagAttribute : MatchSimpleTwoWordTagAttribute
	{
		#region Properties

		/// <summary>
		/// Gets the name of the tag (its identifier and sql text)
		/// </summary>
		protected override string Name
		{
			get
			{
				return OrderByTag.cTagName;
			}
		}

		/// <summary>
		/// Gets the first word of the tag.
		/// </summary>
		protected override string FirstWord 
		{
			get
			{
				return OrderByTag.cTagFirstPart;
			}
		}

		/// <summary>
		/// Gets the second word of the tag.
		/// </summary>
		protected override string SecondWord
		{
			get
			{
				return OrderByTag.cTagSecondPart;
			}
		}

		#endregion
	}

	#endregion
}
