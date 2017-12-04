using System;
using System.Collections.Generic;
using System.Text;

namespace Parser
{
	#region SelectTag

	[TagType(SelectTag.cTagName)]
	[MatchSelectTag]
	internal class SelectTag : SimpleOneWordTag
	{
		#region Consts

		/// <summary>
		/// The name of the tag (its identifier).
		/// </summary>
		public const string cTagName = "SELECT";

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

	#region MatchSelectTagAttribute

	internal class MatchSelectTagAttribute : MatchSimpleOneWordTagAttribute
	{
		#region Properties

		/// <summary>
		/// Gets the name of the tag (its identifier and sql text)
		/// </summary>
		protected override string Name
		{
			get 
			{
				return SelectTag.cTagName;
			}
		}

		#endregion
	}

	#endregion
}
