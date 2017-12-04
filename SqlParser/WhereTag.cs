using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region WhereTag

	[TagType(WhereTag.cTagName)]
	[MatchWhereTag]
	internal class WhereTag : SimpleOneWordTag
	{
		#region Consts

		/// <summary>
		/// The name of the tag (its identifier).
		/// </summary>
		public const string cTagName = "WHERE";

		#endregion

		#region Properties

		/// <summary>
		/// Gets the name of the tag (its identifier and sql text)
		/// </summary>
		protected override string Name
		{
			get
			{
				return cTagName;
			}
		}

		#endregion
	}

	#endregion

	#region MatchWhereTagAttribute

	internal class MatchWhereTagAttribute : MatchSimpleOneWordTagAttribute
	{
		#region Properties

		/// <summary>
		/// Gets the name of the tag (its identifier and sql text)
		/// </summary>
		protected override string Name
		{
			get
			{
				return WhereTag.cTagName;
			}
		}

		#endregion
	}

	#endregion
}
