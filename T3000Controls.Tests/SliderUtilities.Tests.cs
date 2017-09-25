namespace T3000Controls.Tests
{
    using NUnit.Framework;

    [TestFixture]
    class SliderUtilitiesTests
    {
        [Test]
        public void YToValue_Direct()
        {
            //Y from 0 to 100, TopValue = 0, BottomValue = 100, Height = 100
            Assert.AreEqual(0, SliderUtilities.YToValue(0, 0, 100, 100));
            Assert.AreEqual(5, SliderUtilities.YToValue(5, 0, 100, 100));
            Assert.AreEqual(50, SliderUtilities.YToValue(50, 0, 100, 100));
            Assert.AreEqual(95, SliderUtilities.YToValue(95, 0, 100, 100));
            Assert.AreEqual(100, SliderUtilities.YToValue(100, 0, 100, 100));

            //Y from 0 to 200, TopValue = 0, BottomValue = 100, Height = 200
            Assert.AreEqual(0, SliderUtilities.YToValue(0, 0, 100, 200));
            Assert.AreEqual(5, SliderUtilities.YToValue(10, 0, 100, 200));
            Assert.AreEqual(50, SliderUtilities.YToValue(100, 0, 100, 200));
            Assert.AreEqual(95, SliderUtilities.YToValue(190, 0, 100, 200));
            Assert.AreEqual(100, SliderUtilities.YToValue(200, 0, 100, 200));

            //Y from 0 to 100, TopValue = 100, BottomValue = 200, Height = 100
            Assert.AreEqual(100, SliderUtilities.YToValue(0, 100, 200, 100));
            Assert.AreEqual(105, SliderUtilities.YToValue(5, 100, 200, 100));
            Assert.AreEqual(150, SliderUtilities.YToValue(50, 100, 200, 100));
            Assert.AreEqual(195, SliderUtilities.YToValue(95, 100, 200, 100));
            Assert.AreEqual(200, SliderUtilities.YToValue(100, 100, 200, 100));
        }

        [Test]
        public void YToValue_Inverse()
        {
            //Y from 0 to 100, TopValue = 100, BottomValue = 0, Height = 100
            Assert.AreEqual(100, SliderUtilities.YToValue(0, 100, 0, 100));
            Assert.AreEqual(95, SliderUtilities.YToValue(5, 100, 0, 100));
            Assert.AreEqual(50, SliderUtilities.YToValue(50, 100, 0, 100));
            Assert.AreEqual(5, SliderUtilities.YToValue(95, 100, 0, 100));
            Assert.AreEqual(0, SliderUtilities.YToValue(100, 100, 0, 100));

            //Y from 0 to 200, TopValue = 100, BottomValue = 0, Height = 200
            Assert.AreEqual(100, SliderUtilities.YToValue(0, 100, 0, 200));
            Assert.AreEqual(95, SliderUtilities.YToValue(10, 100, 0, 200));
            Assert.AreEqual(50, SliderUtilities.YToValue(100, 100, 0, 200));
            Assert.AreEqual(5, SliderUtilities.YToValue(190, 100, 0, 200));
            Assert.AreEqual(0, SliderUtilities.YToValue(200, 100, 0, 200));

            //Y from 0 to 100, TopValue = 200, BottomValue = 100, Height = 100
            Assert.AreEqual(200, SliderUtilities.YToValue(0, 200, 100, 100));
            Assert.AreEqual(195, SliderUtilities.YToValue(5, 200, 100, 100));
            Assert.AreEqual(150, SliderUtilities.YToValue(50, 200, 100, 100));
            Assert.AreEqual(105, SliderUtilities.YToValue(95, 200, 100, 100));
            Assert.AreEqual(100, SliderUtilities.YToValue(100, 200, 100, 100));
        }

        [Test]
        public void ValueToY_Direct()
        {
            //Value from 0 to 100, TopValue = 0, BottomValue = 100, Height = 100
            Assert.AreEqual(0, SliderUtilities.ValueToY(0, 0, 100, 100));
            Assert.AreEqual(5, SliderUtilities.ValueToY(5, 0, 100, 100));
            Assert.AreEqual(50, SliderUtilities.ValueToY(50, 0, 100, 100));
            Assert.AreEqual(95, SliderUtilities.ValueToY(95, 0, 100, 100));
            Assert.AreEqual(100, SliderUtilities.ValueToY(100, 0, 100, 100));

            //Value from 0 to 100, TopValue = 0, BottomValue = 100, Height = 200
            Assert.AreEqual(0, SliderUtilities.ValueToY(0, 0, 100, 200));
            Assert.AreEqual(10, SliderUtilities.ValueToY(5, 0, 100, 200));
            Assert.AreEqual(100, SliderUtilities.ValueToY(50, 0, 100, 200));
            Assert.AreEqual(190, SliderUtilities.ValueToY(95, 0, 100, 200));
            Assert.AreEqual(200, SliderUtilities.ValueToY(100, 0, 100, 200));

            //Value from 100 to 200, TopValue = 100, BottomValue = 200, Height = 100
            Assert.AreEqual(0, SliderUtilities.ValueToY(100, 100, 200, 100));
            Assert.AreEqual(5, SliderUtilities.ValueToY(105, 100, 200, 100));
            Assert.AreEqual(50, SliderUtilities.ValueToY(150, 100, 200, 100));
            Assert.AreEqual(95, SliderUtilities.ValueToY(195, 100, 200, 100));
            Assert.AreEqual(100, SliderUtilities.ValueToY(200, 100, 200, 100));
        }

        [Test]
        public void ValueToY_Inverse()
        {
            //Value from 0 to 100, TopValue = 100, BottomValue = 0, Height = 100
            Assert.AreEqual(100, SliderUtilities.ValueToY(0, 100, 0, 100));
            Assert.AreEqual(95, SliderUtilities.ValueToY(5, 100, 0, 100));
            Assert.AreEqual(50, SliderUtilities.ValueToY(50, 100, 0, 100));
            Assert.AreEqual(5, SliderUtilities.ValueToY(95, 100, 0, 100));
            Assert.AreEqual(0, SliderUtilities.ValueToY(100, 100, 0, 100));

            //Value from 0 to 100, TopValue = 100, BottomValue = 0, Height = 200
            Assert.AreEqual(200, SliderUtilities.ValueToY(0, 100, 0, 200));
            Assert.AreEqual(190, SliderUtilities.ValueToY(5, 100, 0, 200));
            Assert.AreEqual(100, SliderUtilities.ValueToY(50, 100, 0, 200));
            Assert.AreEqual(10, SliderUtilities.ValueToY(95, 100, 0, 200));
            Assert.AreEqual(0, SliderUtilities.ValueToY(100, 100, 0, 200));

            //Value from 100 to 200, TopValue = 200, BottomValue = 100, Height = 100
            Assert.AreEqual(100, SliderUtilities.ValueToY(100, 200, 100, 100));
            Assert.AreEqual(95, SliderUtilities.ValueToY(105, 200, 100, 100));
            Assert.AreEqual(50, SliderUtilities.ValueToY(150, 200, 100, 100));
            Assert.AreEqual(5, SliderUtilities.ValueToY(195, 200, 100, 100));
            Assert.AreEqual(0, SliderUtilities.ValueToY(200, 200, 100, 100));
        }

        [Test]
        public void ValueToHeight_Direct()
        {
            //Value from 0 to 100, TopValue = 0, BottomValue = 100, Height = 100
            Assert.AreEqual(0, SliderUtilities.ValueToHeight(0, 0, 100, 100));
            Assert.AreEqual(5, SliderUtilities.ValueToHeight(5, 0, 100, 100));
            Assert.AreEqual(50, SliderUtilities.ValueToHeight(50, 0, 100, 100));
            Assert.AreEqual(95, SliderUtilities.ValueToHeight(95, 0, 100, 100));
            Assert.AreEqual(100, SliderUtilities.ValueToHeight(100, 0, 100, 100));

            //Value from 0 to 100, TopValue = 0, BottomValue = 100, Height = 200
            Assert.AreEqual(0, SliderUtilities.ValueToHeight(0, 0, 100, 200));
            Assert.AreEqual(10, SliderUtilities.ValueToHeight(5, 0, 100, 200));
            Assert.AreEqual(100, SliderUtilities.ValueToHeight(50, 0, 100, 200));
            Assert.AreEqual(190, SliderUtilities.ValueToHeight(95, 0, 100, 200));
            Assert.AreEqual(200, SliderUtilities.ValueToHeight(100, 0, 100, 200));

            //Value from 100 to 200, TopValue = 100, BottomValue = 200, Height = 100
            Assert.AreEqual(100, SliderUtilities.ValueToHeight(100, 100, 200, 100));
            Assert.AreEqual(105, SliderUtilities.ValueToHeight(105, 100, 200, 100));
            Assert.AreEqual(150, SliderUtilities.ValueToHeight(150, 100, 200, 100));
            Assert.AreEqual(195, SliderUtilities.ValueToHeight(195, 100, 200, 100));
            Assert.AreEqual(200, SliderUtilities.ValueToHeight(200, 100, 200, 100));
        }

        [Test]
        public void ValueToHeight_Inverse()
        {
            //Value from 0 to 100, TopValue = 100, BottomValue = 0, Height = 100
            Assert.AreEqual(0, SliderUtilities.ValueToHeight(0, 100, 0, 100));
            Assert.AreEqual(5, SliderUtilities.ValueToHeight(5, 100, 0, 100));
            Assert.AreEqual(50, SliderUtilities.ValueToHeight(50, 100, 0, 100));
            Assert.AreEqual(95, SliderUtilities.ValueToHeight(95, 100, 0, 100));
            Assert.AreEqual(100, SliderUtilities.ValueToHeight(100, 100, 0, 100));

            //Value from 0 to 100, TopValue = 100, BottomValue = 0, Height = 200
            Assert.AreEqual(0, SliderUtilities.ValueToHeight(0, 100, 0, 200));
            Assert.AreEqual(10, SliderUtilities.ValueToHeight(5, 100, 0, 200));
            Assert.AreEqual(100, SliderUtilities.ValueToHeight(50, 100, 0, 200));
            Assert.AreEqual(190, SliderUtilities.ValueToHeight(95, 100, 0, 200));
            Assert.AreEqual(200, SliderUtilities.ValueToHeight(100, 100, 0, 200));

            //Value from 100 to 200, TopValue = 200, BottomValue = 100, Height = 100
            Assert.AreEqual(100, SliderUtilities.ValueToHeight(100, 200, 100, 100));
            Assert.AreEqual(105, SliderUtilities.ValueToHeight(105, 200, 100, 100));
            Assert.AreEqual(150, SliderUtilities.ValueToHeight(150, 200, 100, 100));
            Assert.AreEqual(195, SliderUtilities.ValueToHeight(195, 200, 100, 100));
            Assert.AreEqual(200, SliderUtilities.ValueToHeight(200, 200, 100, 100));
        }

        [Test]
        public void GetOffsetValueForValue_Direct()
        {
            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(5, 0, 100));
            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(5, 100, 200));

            Assert.AreEqual(2, SliderUtilities.GetOffsetValueForValue(5, 3, 100));
            Assert.AreEqual(2, SliderUtilities.GetOffsetValueForValue(5, 103, 200));

            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(10, 0, 100));
            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(10, 100, 200));

            Assert.AreEqual(5, SliderUtilities.GetOffsetValueForValue(10, 5, 100));
            Assert.AreEqual(5, SliderUtilities.GetOffsetValueForValue(10, 105, 200));
        }

        [Test]
        public void GetOffsetValueForValue_Inserse()
        {
            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(5, 100, 0));
            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(5, 200, 100));

            Assert.AreEqual(2, SliderUtilities.GetOffsetValueForValue(5, 97, 0));
            Assert.AreEqual(2, SliderUtilities.GetOffsetValueForValue(5, 197, 100));

            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(10, 100, 0));
            Assert.AreEqual(0, SliderUtilities.GetOffsetValueForValue(10, 200, 100));

            Assert.AreEqual(5, SliderUtilities.GetOffsetValueForValue(10, 95, 0));
            Assert.AreEqual(5, SliderUtilities.GetOffsetValueForValue(10, 195, 100));

            Assert.AreEqual(3, SliderUtilities.GetOffsetValueForValue(10, 83, 3));
        }

        [Test]
        public void GetOffsetForValue_Direct()
        {
            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(5, 0, 100, 100), 0.5);
            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(5, 100, 200, 100), 0.5);

            Assert.AreEqual(2, SliderUtilities.GetOffsetForValue(5, 3, 100, 100), 0.5);
            Assert.AreEqual(2, SliderUtilities.GetOffsetForValue(5, 103, 200, 100), 0.5);

            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(10, 0, 100, 100), 0.5);
            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(10, 100, 200, 100), 0.5);

            Assert.AreEqual(5, SliderUtilities.GetOffsetForValue(10, 5, 100, 100), 0.5);
            Assert.AreEqual(5, SliderUtilities.GetOffsetForValue(10, 105, 200, 100), 0.5);
        }

        [Test]
        public void GetOffsetForValue_Inserse()
        {
            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(5, 100, 0, 100), 0.5);
            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(5, 200, 100, 100), 0.5);

            Assert.AreEqual(2, SliderUtilities.GetOffsetForValue(5, 97, 0, 100), 0.5);
            Assert.AreEqual(2, SliderUtilities.GetOffsetForValue(5, 197, 100, 100), 0.5);

            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(10, 100, 0, 100), 0.5);
            Assert.AreEqual(0, SliderUtilities.GetOffsetForValue(10, 200, 100, 100), 0.5);

            Assert.AreEqual(5, SliderUtilities.GetOffsetForValue(10, 95, 0, 100), 0.5);
            Assert.AreEqual(5, SliderUtilities.GetOffsetForValue(10, 195, 100, 100), 0.5);
        }

        [Test]
        public void RoundUp()
        {
            Assert.AreEqual(90, SliderUtilities.RoundUp(81, 100.0F, 0.05F, 10), 0.01);
            Assert.AreEqual(100, SliderUtilities.RoundUp(81, 100.0F, 0.15F, 10), 0.01);
            Assert.AreEqual(20, SliderUtilities.RoundUp(1, 100.0F, 0.15F, 10), 0.01);
        }

        [Test]
        public void RoundDown()
        {
            Assert.AreEqual(70, SliderUtilities.RoundDown(81, 100.0F, 0.05F, 10), 0.01);
            Assert.AreEqual(60, SliderUtilities.RoundDown(81, 100.0F, 0.15F, 10), 0.01);
            Assert.AreEqual(-20, SliderUtilities.RoundDown(1, 100.0F, 0.15F, 10), 0.01);
        }
    }
}
