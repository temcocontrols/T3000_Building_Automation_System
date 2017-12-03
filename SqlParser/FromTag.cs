using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region FromTag

	[TagType(FromTag.cTagName)]
	[MatchFromTag]
	internal class FromTag : SimpleOneWordTag
	{
		#region Consts

		/// <summary>
		/// The name of the tag (its identifier).
		/// </summary>
		public const string cTagName = "FROM";

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

	#region MatchFromTagAttribute

	internal class MatchFromTagAttribute : MatchSimpleOneWordTagAttribute
	{
		#region Properties

		/// <summary>
		/// Gets the name of the tag (its identifier and sql text)
		/// </summary>
		protected override string Name
		{
			get
			{
				return FromTag.cTagName;
			}
		}

		#endregion
	}

	#endregion
}
